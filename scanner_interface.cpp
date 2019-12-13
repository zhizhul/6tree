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
    // Be used in function2_G.cpp.

    // Output pre-defined scanner parameters into the configuration file.
    // Afterwards, users can adjust parameters in the file to generate a different scan command, but incorrect adjustments might trigger errors.
    
    ofstream outfile;
    outfile.open("./" + outdir_name + "/" + _SCANNER_FILE);

    outfile << "app_name : " << _SI_APP_NAME << endl;
    outfile << "ins_num : " << _SI_INS_NUM << endl;
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
    // Be used in function4_R.cpp.

    // Read scanner parameters to generate a scan command for address search.

    ifstream pfile;
    string line;
    scanner_cmd = "";
    pfile.open("./" + treedir_name + "/" + _SCANNER_FILE);
    
    // Read the application name.
    getline(pfile, line);
    vector<string> split_res = f1_str_split(line, ":");
    string app_str = split_res[1];
    split_res.clear();
    f3_trim(app_str);
    scanner_cmd += app_str;

    // Read the number of secondary instructions.
    getline(pfile, line);
    split_res = f1_str_split(line, ":");
    string num_str = split_res[1];
    split_res.clear();
    f3_trim(num_str);
    int ins_num = atoi(num_str.c_str());

    // Read secondary instructions.
    for (int i = 0; i < ins_num; i++)
    {
        getline(pfile, line);
        split_res = f1_str_split(line, ":");
        string ins_str = split_res[1];
        split_res.clear();
        f3_trim(ins_str);
        scanner_cmd += " ";
        scanner_cmd += ins_str;
    }

    pfile.close();
}

void si_network_scan(struct RegionTreeNode **regn_forest, int tree_num, int &budget, ofstream &addr_total_res)
{
    // 根据区树森林写入目标地址集到_SI_STEP_TF_FILE文件中

    // 调用命令行，启动扫描器命令执行扫描

    // 收集结果到数组中，进行排序去重，然后更新收集结果文件，更新addr_total_res，减预算。

    // ----
    // Be used in function4_R.cpp.
}
