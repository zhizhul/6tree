//
//  scanner_interface.cpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/12/12.
//

// Define scanner interface functions, parameters, variables ... in the file.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include "definition.hpp"
#include "function1_T.hpp"
#include "function2_G.hpp"
#include "function3_L.hpp"
#include "function4_R.hpp"
#include "scanner_interface.hpp"

using namespace std;

// Scan command
// In the default configuration, the scan command will be:
// zmap --probe-module=icmp6_echoscan --ipv6-target-file=targets.txt --output-file=result.txt --ipv6-source-ip=2001:1111::1111:2f --bandwidth=10M --cooldown-time=4
string scanner_cmd;

void si_output_scanner_command(string outdir_name)
{
    // Be used in function2_G.cpp=>f2_work().

    // Output pre-defined scanner parameters into the configuration file.
    // Afterwards, users can adjust parameters in the file to generate a different scan command, but incorrect adjustments might trigger errors.
    
    ofstream outfile;
    outfile.open("./" + outdir_name + "/" + _SCANNER_FILE);

    outfile << "app_name : " << _SI_APP_NAME << endl;
    outfile << "ins_num : " << _SI_DFT_INS_NUM << endl;
    outfile << _SI_DFT_INS_PM << endl;
    outfile << _SI_DFT_INS_STEP_TF << endl;
    outfile << _SI_DFT_INS_STEP_RES << endl;
    outfile << _SI_DFT_INS_SIP << endl;
    outfile << _SI_DFT_INS_BW << endl;
    outfile << _SI_DFT_INS_CT << endl;

    outfile.close();
}

void si_read_scanner_command(string treedir_name)
{
    // Be used in function4_R.cpp=>f4_work().

    // Read scanner parameters to generate a scan command for address search.

    ifstream pfile;
    string line;
    scanner_cmd = "";
    pfile.open("./" + treedir_name + "/" + _SCANNER_FILE);
    
    // Read the application name.
    getline(pfile, line);
    vector<string> split_res = f1_str_split(line, ':');
    string app_str = split_res[1];
    split_res.clear();
    f3_trim(app_str);
    scanner_cmd += app_str;

    // Read the number of secondary instructions.
    getline(pfile, line);
    split_res = f1_str_split(line, ':');
    string num_str = split_res[1];
    split_res.clear();
    f3_trim(num_str);
    int ins_num = atoi(num_str.c_str());

    // Read secondary instructions.
    for (int i = 0; i < ins_num; i++)
    {
        getline(pfile, line);
        split_res = f1_str_split(line, ':');
        string ins_str = split_res[1];
        split_res.clear();
        f3_trim(ins_str);
        scanner_cmd += " ";
        scanner_cmd += ins_str;
    }

    pfile.close();
}

void si_write_targets(string *arr, int &arr_idx, string expression, int start_idx, int dimensionality)
{
    // Be used in scanner_interface.cpp=>si_write_on_leaf_nodes().

    int idx;
    for (idx = start_idx; idx < dimensionality; idx++)
    {
        if (expression[idx] == '*')
        {
            break;
        }
    }
    if (idx == dimensionality)
    {
        arr[arr_idx++] = expression;
        return ;
    }
    for (int i = 0; i < base_num; i++)
    {
        if (i < 10)
        {
            expression[idx] = '0' + i;
        }
        else // i >= 10
        {
            expression[idx] = 'a' + i - 10;
        }
        si_write_targets(arr, arr_idx, expression, idx + 1, dimensionality);
    }
}

void si_write_on_leaf_nodes(string *arr, int &arr_idx, struct RegionTreeNode *node)
{
    // Be used in scanner_interface.cpp=>si_network_scan().

    if (node->is_leaf == true)
    {
        int dimensionality = f2_get_dimensionality(base_num);
        si_write_targets(arr, arr_idx, node->expression, 0, dimensionality);
    }
    for (int i = 0; i < node->children_num; i++)
    {
        if (node->children[i] != NULL)
        {
            si_write_on_leaf_nodes(arr, arr_idx, node->children[i]);
        }
    }
}

int si_TSs_scale(struct RegionTreeNode **regn_forest, int tree_num)
{
    // Be used in scanner_interface.cpp=>si_network_scan().

    int target_num = 0;
    int dimensionality = f2_get_dimensionality(base_num);
    for (int i = 0; i < tree_num; i++)
    {
        int star_num = 0;
        string expr = regn_forest[i]->expression;
        for (int j = 0; j < dimensionality; j++)
        {
            if (expr[j] == '*')
            {
                star_num++;
            }
        }
        target_num += pow(base_num, star_num);
    }
    return target_num;
}

int si_network_scan(struct RegionTreeNode **regn_forest, int tree_num, int &budget, ofstream &addr_total_res)
{
    // Be used in function4_R.cpp=>f4_network_scan_feedback().

    // Return the number of discovered addresses.

    // 1. Write targets into _SI_STEP_TF_FILE and deduct the budget.
    int TSs_scale = si_TSs_scale(regn_forest, tree_num);
    string *arr = new string [TSs_scale];
    int arr_idx = 0;
    for (int i = 0; i < tree_num; i++)
    {
        si_write_on_leaf_nodes(arr, arr_idx, regn_forest[i]);
    }
    int arr_scale = arr_idx;

    // -- need work: 到时候可以用Xcode检查一下，看地址的输出是否正常，arr_idx的运行是否正常
    // -- below is test code
    cout << "the scale is: " << arr_scale << endl;
    // -- up is test code
    budget -= arr_scale;

    random_device rd_dev;
    mt19937 gn(rd_dev());
    shuffle(arr, arr + arr_scale, gn);
    
    ofstream target_file;
    target_file.open(_SI_STEP_TF_FILE);
    for (int i = 0; i < arr_scale; i++)
    {
        target_file << f3_addr_tran_std(arr[i]) << endl;
    }
    target_file.close();

    // 2. Call the scanner ZMapv6 to search active addresses.
    // -- need work: 测试的时候可以把这个代码注释掉，那么就可以进行本地测试，看正常工作能力。
    system(scanner_cmd.c_str());

    // 3. Sort and unique the search result, since some addresses might be repeated (multi response packets).
    string line;
    ifstream res_file_read;
    res_file_read.open(_SI_STEP_RES_FILE);
    arr_scale = 0;
    while (getline(res_file_read, line))
    {
        arr[arr_scale++] = f1_std_tran_b4(line);
    }
    res_file_read.close();

    sort(arr, arr + arr_scale, f1_str_cmp);
    int new_arr_scale = (int )(unique(arr, arr + arr_scale) - arr);
    arr_scale = new_arr_scale;

    ofstream res_file_write;
    res_file_write.open(_SI_STEP_RES_FILE);
    for (int i = 0; i < arr_scale; i++)
    {
        string std_str = f1_b4_tran_std(arr[i]);
        res_file_write << std_str << endl;
        addr_total_res << std_str << endl;
    }
    res_file_write.close();

    delete [] arr;

    int NDA = arr_scale;
    return NDA;
}

int si_adet_network_scan(string *targets, int targets_num, int &budget)
{
    // Be used in function4_R.cpp=>f4_adet_scan_feedback().

    // Return the number of discovered addresses.

    // 1. Write targets into _SI_STEP_TF_FILE and deduct the budget.
    ofstream target_file;
    target_file.open(_SI_STEP_TF_FILE);
    for (int i = 0; i < targets_num; i++)
    {
        target_file << f3_addr_tran_std(targets[i]) << endl;
    }
    target_file.close();

    // 2. Call the scanner ZMapv6 to search active addresses.
    system(scanner_cmd.c_str());

    // 3. Count the number of discovered addresses and return it.
    int NDA = 0;
    string line;
    ifstream res_file;
    res_file.open(_SI_STEP_RES_FILE);
    while (getline(res_file, line))
    {
        NDA++;
    }
    res_file.close();

    return NDA;
}
