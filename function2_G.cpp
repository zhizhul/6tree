//
//  function2_G.cpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/12/10.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include "definition.hpp"
#include "function1_T.hpp"
#include "function2_G.hpp"
#include "scanner_interface.hpp"

using namespace std;

int f2_get_base_num(int in_type)
{
    if (in_type == _INS_INB1) return 2; 
    else if (in_type == _INS_INB2) return 4; 
    else if (in_type == _INS_INB3) return 8; 
    else if (in_type == _INS_INB4) return 16; 
    else return 32; // in_type == _INS_INB5
}

int f2_get_dimensionality(int base_num)
{
    if (base_num == 2) return 128; 
    else if (base_num == 4) return 64; 
    else if (base_num == 8) return 42; 
    else if (base_num == 16) return 32; 
    else return 25; // base_num == 32
}

void f2_DHC(int base_num, struct SpaceTreeNode *node, string *arr, int &node_num)
{
    int node_vector_num = node->sup - node->inf + 1;
    if (node_vector_num <= base_num)
    {
        return ;
    }
    
    int dimensionality = f2_get_dimensionality(base_num);
    int var_dimen = 0;
    int inf = node->inf;
    int sup = node->sup;
    for (int i = 0; i < dimensionality; i++)
    {
        bool find = false;
        for (int j = inf; j < sup; j++)
        {
            if (arr[j][i] != arr[j + 1][i])
            {
                find = true;
                var_dimen = i;
                break;
            }
        }
        if (find == true)
        {
            break;
        }
    }
    
    int children_num = 1;
    for (int i = inf; i < sup; i++)
    {
        if (arr[i][var_dimen] != arr[i + 1][var_dimen])
        {
            children_num++;
        }
    }
    node->children = new SpaceTreeNode *[children_num];
    node->children_num = children_num;
    int step = 0;
    node->children[step] = new SpaceTreeNode;
    node->children[step]->parent = node;
    node->children[step]->children = NULL;
    node->children[step]->children_num = 0;
    node->children[step]->inf = inf;
    for (int i = inf; i < sup; i++)
    {
        if (arr[i][var_dimen] != arr[i + 1][var_dimen])
        {
            node->children[step++]->sup = i;
            node->children[step] = new SpaceTreeNode;
            node->children[step]->parent = node;
            node->children[step]->children = NULL;
            node->children[step]->children_num = 0;
            node->children[step]->inf = i + 1;
        }
    }
    node->children[step]->sup = sup;
    
    node_num += children_num;
    
    for (int i = 0; i < children_num; i++)
    {
        f2_DHC(base_num, node->children[i], arr, node_num);
    }
}

struct SpaceTreeNode *f2_tree_generation(int in_type, string *arr, int arr_scale, int &node_num)
{
    // Space tree generation according to the base mode and the address vector array.
    
    // 1. Initialize the root node.
    struct SpaceTreeNode *root = new struct SpaceTreeNode;
    root->inf = 0;
    root->sup = arr_scale - 1;
    root->parent = NULL;
    root->children = NULL;
    root->children_num = 0;
    
    node_num = 1;
    
    // 2. Recursively generate nodes from the root node.
    int base_num = f2_get_base_num(in_type);
    f2_DHC(base_num, root, arr, node_num);
    
    // 3. Return the result.
    return root;
}

string f2_to_lower(string line, int dimensionality)
{
    string res_str = "";
    for (int i = 0; i < dimensionality; i++)
    {
        if (line[i] >= 'A' && line[i] <= 'Z')
        {
            res_str += line[i] + 'a' - 'A';
        }
        else
        {
            res_str += line[i];
        }
    }
    return res_str;
}

void f2_release_tree(struct SpaceTreeNode *node)
{
    // Release allocated space tree data.
    int children_num = node->children_num;
    for (int i = 0; i < children_num; i++)
    {
        f2_release_tree(node->children[i]);
    }
    if (children_num != 0)
    {
        delete [] node->children;
    }
    delete node;
}

void f2_store_node(struct SpaceTreeNode **node_arr, int &node_arr_scale, struct SpaceTreeNode *node)
{
    int children_num = node->children_num;
    for (int i = 0; i < children_num; i++)
    {
        node_arr[node_arr_scale++] = node->children[i];
        node->children[i]->number = node_arr_scale;
    }
    for (int i = 0; i < children_num; i++)
    {
        f2_store_node(node_arr, node_arr_scale, node->children[i]);
    }
}

void f2_output_tree(struct SpaceTreeNode *root, int node_num, int in_type, string outdir_name)
{
    // Output the generated space tree information. Breadth-first order.
    
    // 1. Store tree nodes into the array.
    struct SpaceTreeNode **node_arr = new struct SpaceTreeNode *[node_num + 10];
    int node_arr_scale = 0;
    node_arr[node_arr_scale++] = root;
    root->number = node_arr_scale;
    f2_store_node(node_arr, node_arr_scale, root);
    
    // 2. Output node information to the tree information file.
    ofstream outfile;
    outfile.open("./" + outdir_name + "/" + _TREE_FILE);
    outfile << "base_num : " << f2_get_base_num(in_type) << endl;
    outfile << "node_num : " << node_num << endl;
    outfile << "num, inf, sup, parent_num, children_num" << endl;
    outfile << "1, " << root->inf << ", " << root->sup << ", 0, " << root->children_num << endl;
    for (int i = 1; i < node_num; i++)
    {
        int num = i + 1;
        int inf = node_arr[i]->inf;
        int sup = node_arr[i]->sup;
        int parent_num = node_arr[i]->parent->number;
        int children_num = node_arr[i]->children_num;
        outfile << num << ", " << inf << ", " << sup << ", " << parent_num << ", " << children_num << endl;
    }
    outfile.close();
    
    delete [] node_arr;
}

void f2_output_arr(string *arr, int arr_scale, string outdir_name)
{
    // Output the vector sequence.
    ofstream outfile;
    outfile.open("./" + outdir_name + "/" + _ARR_FILE);
    for (int i = 0; i < arr_scale; i++)
    {
        outfile << arr[i] << endl;
    }
    outfile.close();
}

void f2_output_search_parameters(string outdir_name)
{
    // Output pre-defined search parameters into the configuration file.
    // Users can adjust parameters in the file.
    ofstream outfile;
    outfile.open("./" + outdir_name + "/" + _SEARCH_FILE);
    outfile << "budget : " << _SCAN_BUDGET << endl;
    outfile << "step_budget : " << _SCAN_STEP_BUDGET << endl;
    outfile << "adet_ptimes : " << _SCAN_TIMES << endl;
    outfile << "adet_tsscale_thd : " << _SCAN_ZETA << endl;
    outfile << "adet_aad_thd : " << _SCAN_PI << endl;
    outfile << "adet_crip : " << _SCAN_CRIP << endl;
    outfile.close();
}

void f2_work(int type1, string str2, int type3, string str4)
{
    // Analyze the instruction.
    string infile_name;
    string outdir_name;
    int in_type;
    if (type1 == _INS_OUTTREE)
    {
        // str2 is the folder name, str4 is the address file name.
        infile_name = str4;
        outdir_name = str2;
        in_type = type3;
    }
    else
    {
        // str4 is the folder name, str2 is the address file name.
        infile_name = str2;
        outdir_name = str4;
        in_type = type1;
    }
    
    f1_print_time();
    cout << "[Generation] Start space tree generation." << endl;
    f1_print_time();
    cout << "[Generation] Input data." << endl;
    
    // Count the number of lines, and build a corresponding array.
    ifstream infile;
    string line;
    int arr_scale = 0;
    infile.open(infile_name);
    while (getline(infile, line))
    {
        arr_scale++;
    }
    infile.close();
    string *arr = new string [arr_scale + 10];
    
    // Get the base number and corresponding vectors.
    int dimensionality = f2_get_dimensionality(f2_get_base_num(in_type));
    arr_scale = 0;
    infile.open(infile_name);
    while (getline(infile, line))
    {
        if (f1_check_intype(in_type, line))
        {
            arr[arr_scale++] = f2_to_lower(line, dimensionality);
        }
    }
    infile.close();
    f1_print_time();
    cout << "[Generation] Input data finished." << endl;
    
    // Generate space tree.
    f1_print_time();
    cout << "[Generation] Generation." << endl;
    int node_num;
    struct SpaceTreeNode *root = f2_tree_generation(in_type, arr, arr_scale, node_num);
    
    f1_print_time();
    cout << "[Generation] Generation finished." << endl;
    f1_print_time();
    cout << "[Generation] Output data." << endl;
    
    // Generate the folder.
    string dir_str = "./" + outdir_name;
    mkdir((const char*)(dir_str.c_str()), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    // Output the results.
    f2_output_tree(root, node_num, in_type, outdir_name);
    f2_output_arr(arr, arr_scale, outdir_name);
    f2_release_tree(root);
    delete[] arr;
    
    // Output pre-defined search parameters.
    f2_output_search_parameters(outdir_name);

    // Output pre-defined scanner parameters.
    si_output_scanner_command(outdir_name);
    
    f1_print_time();
    cout << "[Generation] Output data finished." << endl;
    f1_print_time();
    cout << "[Generation] Space tree generation finished." << endl;
}

void f2_access(int argc, const char * argv[])
{
    // 6tree -G (-in-b* *address file name*) (-out-tree *output folder name*)
    
    // 1. The base modes include binary (b2), quatenary (b2), ..., base-32 (b5). b3 omits the first two bits (00), b5 omits the first three bits (001).
    // 2. String which does not conform to the mode will be omitted.
    // 3. Generated space tree information will be stored in the folder.
    // 4. Inputed addresses must be sorted and unique.
    
    if (argc != 6)
    {
        cout << "[Error] Function instruction is incorrect." << endl;
        return;
    }
    
    // Check instruction correctness.
    int type1 = f1_type_ins(string(argv[2]));
    int type3 = f1_type_ins(string(argv[4]));
    int pool_ins_in = 0;
    int pool_ins_out = 0;

    if (type1 >= _INS_INB1 && type1 <= _INS_INB5) pool_ins_in++; 
    if (type3 >= _INS_INB1 && type3 <= _INS_INB5) pool_ins_in++; 
    if (type1 == _INS_OUTTREE) pool_ins_out++; 
    if (type3 == _INS_OUTTREE) pool_ins_out++; 
    
    if (pool_ins_in != 1 || pool_ins_out != 1)
    {
        cout << "[Error] Function instruction is incorrect." << endl;
        return;
    }
    
    // Start the space tree generation.
    f2_work(type1, string(argv[3]), type3, string(argv[5]));
}
