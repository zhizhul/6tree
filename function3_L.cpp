//
//  function3_L.cpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/3/2.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include "definition.hpp"
#include "function1_T.hpp"
#include "function2_G.hpp"
#include "function3_L.hpp"

using namespace std;

void f3_trim(string &s)
{
    // Delete space characters in the string.
    int index = 0;
    if (!s.empty())
    {
        while ((index = (int )(s.find(' ', index))) != string::npos)
        {
            s.erase(index, 1);
        }
    }
}

void f3_initialize_DS(struct PreparedSpaceTreeNode *node, struct DimenStack *parent_DS, int dimensionality, string *arr)
{
    // Initialize the dimensional stack.
    // Its index starts from 0.
    
    // Inherit the stack from the parent node.
    node->DS.stack = new int[dimensionality];
    int *times = new int[dimensionality];
    for (int i = 0; i < dimensionality; i++)
    {
        times[i] = 0;
    }
    int parent_num = parent_DS->num;
    int num = 0;
    for (int i = 0; i < parent_num; i++)
    {
        node->DS.stack[i] = parent_DS->stack[i];
        times[node->DS.stack[i]]++;
    }
    num = parent_num;
    node->DS.num = num;
    
    // Push new steady dimensions into the DS.
    int inf = node->inf;
    int sup = node->sup;
    for (int i = 0; i < dimensionality; i++)
    {
        if (times[i] == 0)
        {
            bool steady = true;
            for (int j = inf; j < sup; j++)
            {
                if (arr[j][i] != arr[j + 1][i])
                {
                    steady = false;
                    break;
                }
            }
            if (steady == true)
            {
                node->DS.stack[num++] = i;
                times[i]++;
            }
        }
    }
    node->DS.num = num;
    int children_num = node->children_num;
    for (int i = 0; i < children_num; i++)
    {
        f3_initialize_DS(node->children[i], &(node->DS), dimensionality, arr);
    }
    
    // For leaf nodes, push remaining dimensions.
    if (children_num == 0)
    {
        for (int i = 0; i < dimensionality; i++)
        {
            if (times[i] == 0)
            {
                node->DS.stack[num++] = i;
                times[i]++;
            }
        }
    }
    node->DS.num = num;
    
    delete [] times;
}

int f3_DS_pop(struct PreparedSpaceTreeNode *node)
{
    // Pop a dimension from the DS.
    int num = node->DS.num;
    num--;
    int res = node->DS.stack[num];
    node->DS.num = num;
    return res;
}

void f3_TS_expand(struct PreparedSpaceTreeNode *node, int dimen)
{
    // Expand the TS based on a dimension.
    int num = node->TS.num;
    string *exps = node->TS.expressions;
    for (int i = 0; i < num; i++)
    {
        exps[i][dimen] = '*';
    }
    sort(exps, exps + num, f1_str_cmp);
    int new_num;
    new_num = (int)(unique(exps, exps + num) - exps);
    node->TS.num = new_num;
}

void f3_initialize_TS_SS_NDA(struct PreparedSpaceTreeNode **node_arr, int node_num, string *arr)
{
    for (int i = 1; i <= node_num; i++)
    {
        struct PreparedSpaceTreeNode *node = node_arr[i];
        
        node->NDA = 0;
        node->SS.num = 0;
        node->SS.expressions = NULL;
        if (node->children_num == 0)
        {
            int inf = node->inf;
            int sup = node->sup;
            node->TS.num = sup - inf + 1;
            node->TS.expressions = new string [sup - inf + 1];
            int step = 0;
            for (int j = inf; j <= sup; j++)
            {
                node->TS.expressions[step++] = arr[j];
            }
            
            int dimen = f3_DS_pop(node);
            f3_TS_expand(node, dimen);
        }
        else
        {
            node->TS.num = 0;
            node->TS.expressions = NULL;
        }
    }
}

struct PreparedSpaceTreeNode *f3_prepare_space_tree(string treedir_name)
{
    // Read the space tree information, and configure necessary data structures.
    
    ifstream treefile;
    string line;
    treefile.open("./" + treedir_name + "/" + _TREE_FILE);
    
    // Read the base number.
    getline(treefile, line);
    vector<string> split_res = f1_str_split(line, ':');
    string num_str = split_res[1];
    split_res.clear();
    f3_trim(num_str);
    base_num = atoi(num_str.c_str());
    
    // Read the number of space tree nodes.
    getline(treefile, line);
    split_res = f1_str_split(line, ':');
    num_str = split_res[1];
    split_res.clear();
    f3_trim(num_str);
    int node_num = atoi(num_str.c_str());
    
    // Generate a node array, then read node information.
    // Its index starts from 1.
    struct PreparedSpaceTreeNode **node_arr = new struct PreparedSpaceTreeNode *[node_num + 10];
    getline(treefile, line);
    while (getline(treefile, line))
    {
        split_res = f1_str_split(line, ',');
        string str = split_res[0];
        f3_trim(str);
        int num = atoi(str.c_str());
        str = split_res[1];
        f3_trim(str);
        int inf = atoi(str.c_str());
        str = split_res[2];
        f3_trim(str);
        int sup = atoi(str.c_str());
        str = split_res[3];
        f3_trim(str);
        int parent_num = atoi(str.c_str());
        str = split_res[4];
        f3_trim(str);
        split_res.clear();
        int children_num = atoi(str.c_str());
        
        node_arr[num] = new struct PreparedSpaceTreeNode;
        node_arr[num]->inf = inf;
        node_arr[num]->sup = sup;
        
        if (parent_num == 0)
        {
            node_arr[num]->parent = NULL;
        }
        else
        {
            node_arr[num]->parent = node_arr[parent_num];
            int original_children_num = node_arr[parent_num]->children_num;
            node_arr[parent_num]->children[original_children_num] = node_arr[num];
            node_arr[parent_num]->children_num++;
        }
        
        node_arr[num]->children_num = 0;
        if (children_num != 0)
        {
            node_arr[num]->children = new struct PreparedSpaceTreeNode *[children_num + 2];
        }
        else
        {
            node_arr[num]->children = NULL;
        }
    }
    treefile.close();
    
    // Read the vector sequence information.
    ifstream vecfile;
    vecfile.open("./" + treedir_name + "/" + _ARR_FILE);
    int arr_scale = 0;
    while (getline(vecfile, line))
    {
        arr_scale++;
    }
    vecfile.close();
    string *arr = new string[arr_scale + 10];
    arr_scale = 0;
    vecfile.open("./" + treedir_name + "/" + _ARR_FILE);
    while (getline(vecfile, line))
    {
        arr[arr_scale++] = line;
    }
    vecfile.close();
    
    // Configure necessary data structures.
    struct PreparedSpaceTreeNode *root = node_arr[1];
    struct DimenStack emptyStack;
    emptyStack.num = 0;
    emptyStack.stack = NULL;
    int dimensionality = f2_get_dimensionality(base_num);
    f3_initialize_DS(root, &emptyStack, dimensionality, arr);
    f3_initialize_TS_SS_NDA(node_arr, node_num, arr);
    
    delete [] node_arr;
    delete [] arr;
    return root;
}

struct SequenceNode *f3_gather_leaves(struct PreparedSpaceTreeNode *node, struct SequenceNode *link_node)
{
    // Generate the leaf node sequence for the pre-scanning.
    
    if (node->children_num == 0)
    {
        struct SequenceNode *new_link_node = new struct SequenceNode;
        new_link_node->node = node;
        new_link_node->next = link_node;
        link_node = new_link_node;
    }
    else
    {
        int children_num = node->children_num;
        for (int i = 0; i < children_num; i++)
        {
            struct SequenceNode *new_link_node = f3_gather_leaves(node->children[i], link_node);
            link_node = new_link_node;
        }
    }
    return link_node;
}

int f3_get_type(int base_num)
{
    if (base_num == 2)
    {
        return _INS_OUTB1;
    }
    else if (base_num == 4)
    {
        return _INS_OUTB2;
    }
    else if (base_num == 8)
    {
        return _INS_OUTB3;
    }
    else if (base_num == 16)
    {
        return _INS_OUTB4;
    }
    else // base_num == 32
    {
        return _INS_OUTB5;
    }
}

struct SearchTreeNode *f3_generate_search_tree(string *arr, int arr_scale)
{
    int dimensionality = f2_get_dimensionality(base_num);
    struct SearchTreeNode *root = new struct SearchTreeNode;
    root->NAA = 0;
    root->level = 0;
    root->ch = '-';
    root->is_leaf = false;
    root->parent = NULL;
    root->children_num = base_num;
    root->children = new struct SearchTreeNode *[base_num];
    for (int i = 0; i < base_num; i++)
    {
        root->children[i] = NULL;
    }
    string str = "";
    for (int i = 0; i < dimensionality; i++)
    {
        str += '*';
    }
    root->expression = str;
    
    for (int i = 0; i < arr_scale; i++)
    {
        string line = arr[i];
        struct SearchTreeNode *sit = root;
        sit->NAA++;
        for (int j = 0; j < dimensionality; j++)
        {
            char ch = line[j];
            int index;
            if (ch >= '0' && ch <= '9')
            {
                index = ch - '0';
            }
            else if (ch >= 'A' && ch <= 'V')
            {
                index = ch - 'A' + 10;
            }
            else // ch >= 'a' && ch <= 'v'
            {
                index = ch - 'a' + 10;
            }
            if (sit->children[index] == NULL)
            {
                struct SearchTreeNode *new_node = new struct SearchTreeNode;
                new_node->NAA = 0;
                new_node->level = j + 1;
                new_node->ch = ch;
                new_node->parent = sit;
                if (new_node->level == dimensionality)
                {
                    new_node->is_leaf = true;
                    new_node->children_num = 0;
                    new_node->children = NULL;
                }
                else
                {
                    new_node->is_leaf = false;
                    new_node->children_num = base_num;
                    new_node->children = new struct SearchTreeNode *[base_num];
                    for (int k = 0; k < base_num; k++)
                    {
                        new_node->children[k] = NULL;
                    }
                }
                new_node->expression = sit->expression;
                new_node->expression[j] = ch;
                sit->children[index] = new_node;
            }
            sit = sit->children[index];
            sit->NAA++;
        }
    }
    
    return root;
}

int f3_testtype_tran_intype(int test_type)
{
    if (test_type == _INS_TESTSTD)
    {
        return _INS_INSTD;
    }
    else if (test_type == _INS_TESTB1)
    {
        return _INS_INB1;
    }
    else if (test_type == _INS_TESTB2)
    {
        return _INS_INB2;
    }
    else if (test_type == _INS_TESTB3)
    {
        return _INS_INB3;
    }
    else if (test_type == _INS_TESTB4)
    {
        return _INS_INB4;
    }
    else // test_type == _INS_TESTB1
    {
        return _INS_INB5;
    }
}

struct SearchTreeNode *f3_prepare_test_data(string testfile_name, int test_type, string treedir_name)
{
    // Read test address information, and configure a search tree for the local simulation scan.
    
    // 1. Count number of addresses, and generate an array.
    ifstream testfile;
    string line;
    int arr_scale = 0;
    testfile.open(testfile_name);
    while (getline(testfile, line))
    {
        arr_scale++;
    }
    testfile.close();
    string *arr = new string[arr_scale + 10];
    
    // 2. Read address vectors.
    arr_scale = 0;
    testfile.open(testfile_name);
    while (getline(testfile, line))
    {
        int in_type = f3_testtype_tran_intype(test_type);
        if (f1_check_intype(in_type, line))
        {
            arr[arr_scale++] = f1_tran_in_b4(in_type, line);
        }
    }
    testfile.close();
    
    /*  // Sort and unique vectors.
     sort(arr, arr + arr_scale, f1_str_cmp);
     int new_arr_scale;
     new_arr_scale = (int)(unique(arr, arr + arr_scale) - arr);
     arr_scale = new_arr_scale;
     */
    
    // 3. Adjust the base mode.
    ifstream treefile;
    treefile.open("./" + treedir_name + "/" + _TREE_FILE);
    getline(treefile, line);
    vector<string> split_res = f1_str_split(line, ':');
    string num_str = split_res[1];
    f3_trim(num_str);
    base_num = atoi(num_str.c_str());
    int tree_type = f3_get_type(base_num);
    treefile.close();
    for (int i = 0; i < arr_scale; i++)
    {
        arr[i] = f1_tran_b4_out(tree_type, arr[i]);
    }
    split_res.clear();
    
    // 4. Generate the search tree and return.
    struct SearchTreeNode *search_root = f3_generate_search_tree(arr, arr_scale);
    delete [] arr;
    
    return search_root;
}

void f3_read_local_scan_parameters(int &budget, int &itn_budget, string treedir_name)
{
    ifstream treefile;
    treefile.open("./" + treedir_name + "/" + _PAR_FILE);
    
    // The budget.
    string line;
    getline(treefile, line);
    vector<string> split_res = f1_str_split(line, ':');
    string num_str = split_res[1];
    f3_trim(num_str);
    budget = atoi(num_str.c_str());
    split_res.clear();
    
    // The step budget.
    getline(treefile, line);
    split_res = f1_str_split(line, ':');
    num_str = split_res[1];
    f3_trim(num_str);
    itn_budget = atoi(num_str.c_str());
    split_res.clear();
    
    treefile.close();
}

void f3_release_region_tree(struct RegionTreeNode *regn_ptr)
{
    if (regn_ptr->is_leaf == true)
    {
        delete regn_ptr;
        return ;
    }
    int children_num = regn_ptr->children_num;
    for (int i = 0; i < children_num; i++)
    {
        if (regn_ptr->children[i] != NULL)
        {
            f3_release_region_tree(regn_ptr->children[i]);
        }
    }
    delete regn_ptr->children;
    delete regn_ptr;
}

void f3_region_tree_subtract(struct RegionTreeNode *regn_root, string expr)
{
    // The deleted region tree node (which corresponds to the SS) pointer will become NULL.
    
    // 1. From regn_root, search the leaf node whose expression is equal to or include expr.
    struct RegionTreeNode *regn_ptr = regn_root;
    while (regn_ptr->is_leaf == false)
    {
        bool find_subseqt_node = false;
        for (int i = 0; i < base_num; i++) // children_num == base_num
        {
            struct RegionTreeNode *chd_ptr = regn_ptr->children[i];
            if (chd_ptr != NULL && f3_expression_belong(expr, chd_ptr->expression))
            {
                regn_ptr = chd_ptr;
                find_subseqt_node = true;
                break;
            }
        }
        if (find_subseqt_node == false)
        {
            // The subtract operation has been performed.
            return ;
        }
    }
    
    // 2. Perform the subtract operation.
    
    // 2.1 Generate/Find the region tree node which corresponds to the SS (expr).
    int dimensionality = f2_get_dimensionality(base_num);
    while (regn_ptr->expression != expr)
    {
        int dft_dimension;
        for (dft_dimension = 0; dft_dimension < dimensionality; dft_dimension++)
        {
            if (regn_ptr->expression[dft_dimension] == '*' && expr[dft_dimension] != '*')
            {
                break;
            }
        }
        regn_ptr->is_leaf = false;
        regn_ptr->children_num = base_num;
        regn_ptr->children = new struct RegionTreeNode *[base_num];
        for (int i = 0; i < base_num; i++)
        {
            regn_ptr->children[i] = new struct RegionTreeNode;
            struct RegionTreeNode *chd_ptr = regn_ptr->children[i];
            
            chd_ptr->expression = regn_ptr->expression;
            if (i < 10)
            {
                chd_ptr->expression[dft_dimension] = i + '0';
            }
            else
            {
                chd_ptr->expression[dft_dimension] = i - 10 + 'a';
            }
            chd_ptr->is_leaf = true;
            chd_ptr->spe_node_ptr = regn_ptr->spe_node_ptr;
            chd_ptr->parent = regn_ptr;
            chd_ptr->children = NULL;
            chd_ptr->children_num = 0;
        }
        
        for (int i = 0; i < base_num; i++)
        {
            if (f3_expression_belong(expr, regn_ptr->children[i]->expression))
            {
                regn_ptr = regn_ptr->children[i];
                break;
            }
        }
    }
    
    // 2.2 Delete the region tree node which corresponds to the SS (expr).
    for (int i = 0; i < base_num; i++)
    {
        if (regn_ptr->parent->children[i] == regn_ptr)
        {
            regn_ptr->parent->children[i] = NULL;
            break;
        }
    }
    f3_release_region_tree(regn_ptr);
}

bool f3_expression_belong(string str1, string str2)
{
    // If str1 belongs to or is equal to str2.
    // For instance, 567*7*8 belongs to 56**7*8, return true.
    // Their lengths must be identical.
    
    int len = (int )str1.length();
    for (int i = 0; i < len; i++)
    {
        if (str1[i] != str2[i] && str2[i] != '*')
        {
            return false;
        }
    }
    return true;
}

void f3_init_region_trees(struct RegionTreeNode **region_forest, int &forest_idx, struct SequenceNode *xi_ptr)
{
    // Initialize the region trees.
    // A root of a region tree corresponds to an expression of a TS of a space tree node.
    
    struct PreparedSpaceTreeNode *spe_node = xi_ptr->node;
    int TS_expression_num = spe_node->TS.num;
    for (int i = 0; i < TS_expression_num; i++)
    {
        region_forest[forest_idx] = new struct RegionTreeNode;
        struct RegionTreeNode *regn_root = region_forest[forest_idx];
        
        // Input the information of TS.
        regn_root->expression = spe_node->TS.expressions[i];
        regn_root->is_leaf = true;
        regn_root->spe_node_ptr = spe_node;
        regn_root->parent = NULL;
        regn_root->children = NULL;
        regn_root->children_num = 0;
        
        // Renew the tree based on the information of SSs.
        int SS_expression_num = spe_node->SS.num;
        for (int j = 0; j < SS_expression_num; j++)
        {
            // If the scanned region belongs to the target region of regn_root, perform a subtract operation.
            string str = spe_node->SS.expressions[j];
            if (f3_expression_belong(str, regn_root->expression))
            {
                f3_region_tree_subtract(regn_root, str);
            }
        }
        
        forest_idx++;
    }
}

string f3_addr_tran_std(string expr)
{
    if (base_num == 2)
    {
        return f1_b4_tran_std(f1_b1_tran_b4(expr));
    }
    else if (base_num == 4)
    {
        return f1_b4_tran_std(f1_b2_tran_b4(expr));
    }
    else if (base_num == 8)
    {
        return f1_b4_tran_std(f1_b3_tran_b4(expr));
    }
    else if (base_num == 16)
    {
        return f1_b4_tran_std(expr);
    }
    else // base_num == 32
    {
        return f1_b4_tran_std(f1_b5_tran_b4(expr));
    }
}

void f3_local_rec_output_addrs(struct SearchTreeNode *sch_node, ofstream &addr_total_res, ofstream &active_addrs)
{
    // Output active addresses which corresponds to leaf nodes from sch_node.
    
    if (sch_node->is_leaf == true)
    {
        addr_total_res << f3_addr_tran_std(sch_node->expression) << endl;
        active_addrs << f3_addr_tran_std(sch_node->expression) << endl;
    }
    else
    {
        for (int i = 0; i < base_num; i++)
        {
            if (sch_node->children[i] != NULL)
            {
                f3_local_rec_output_addrs(sch_node->children[i], addr_total_res, active_addrs);
            }
        }
    }
}

int f3_local_rec_search(string expr, struct SearchTreeNode *sch_node, ofstream &addr_total_res, ofstream &active_addrs)
{
    // Search active addresses through a recursive design.
    // Found active addresses will be stored in addr_total_res and active_addrs based on colon-hexadecimal notation.
    // Return total scanned address number.
    
    int expr_len = (int )expr.length();
    int used_budget;
    if (expr[0] == '*')
    {
        bool all_star = true;
        for (int i = 1; i < expr_len; i++)
        {
            if (expr[i] != '*')
            {
                all_star = false;
                break;
            }
        }
        if (all_star == true)
        {
            f3_local_rec_output_addrs(sch_node, addr_total_res, active_addrs);
            
            used_budget = pow(base_num, expr_len);
            return used_budget;
        }
        else
        {
            used_budget = 0;
            for (int i = 0; i < base_num; i++)
            {
                if (sch_node->children[i] != NULL)
                {
                    used_budget += f3_local_rec_search(expr.substr(1), sch_node->children[i], addr_total_res, active_addrs);
                }
            }
            return used_budget;
        }
    }
    else
    {
        int index;
        if (expr[0] >= 'a' && expr[0] <= 'v')
        {
            index = expr[0] + 10 - 'a';
        }
        else if (expr[0] >= 'A' && expr[0] <= 'V')
        {
            index = expr[0] + 10 - 'A';
        }
        else // expr[0] >= '0' && expr[0] <= '9'
        {
            index = expr[0] - '0';
        }
        
        if (sch_node->children[index] == NULL)
        {
            used_budget = 0;
            return used_budget;
        }
        else
        {
            if (expr_len == 1)
            {
                f3_local_rec_output_addrs(sch_node, addr_total_res, active_addrs);
                
                used_budget = 1;
                return used_budget;
            }
            else
            {
                used_budget = f3_local_rec_search(expr.substr(1), sch_node->children[index], addr_total_res, active_addrs);
                return used_budget;
            }
        }
    }
}

int f3_scan_on_leaves(struct RegionTreeNode *regn_node, struct SearchTreeNode *A, ofstream &addr_total_res, ofstream &active_addrs)
{
    // Found active addresses will be stored in addr_total_res and active_addrs based on colon-hexadecimal notation.
    // Return total scanned address number.
    if (regn_node->is_leaf == true)
    {
        return f3_local_rec_search(regn_node->expression, A, addr_total_res, active_addrs);
    }
    else
    {
        int children_num = regn_node->children_num;
        int used_budget = 0;
        for (int i = 0; i < children_num; i++)
        {
            if (regn_node->children[i] != NULL)
            {
                used_budget += f3_scan_on_leaves(regn_node->children[i], A, addr_total_res, active_addrs);
            }
        }
        return used_budget;
    }
}

void f3_local_scan(struct RegionTreeNode **regn_forest, int tree_num, int &budget, ofstream &addr_total_res, ofstream &active_addrs, struct SearchTreeNode *A)
{
    // Perform a local scan, the found active addresses will be stored both in addr_total_res and active_addrs.
    for (int i = 0; i < tree_num; i++)
    {
        struct RegionTreeNode *regn_tree = regn_forest[i];
        budget -= f3_scan_on_leaves(regn_tree, A, addr_total_res, active_addrs);
    }
}

void f3_copy_TS2SS(struct PreparedSpaceTreeNode *spe_node)
{
    if (spe_node->SS.expressions != NULL)
    {
        delete [] spe_node->SS.expressions;
    }
    int SS_num = spe_node->TS.num;
    spe_node->SS.num = SS_num;
    spe_node->SS.expressions = new string [SS_num];
    for (int i = 0; i < SS_num; i++)
    {
        spe_node->SS.expressions[i] = spe_node->TS.expressions[i];
    }
}

string f3_std_tran_addr(string expr)
{
    if (base_num == 2)
    {
        return f1_b4_tran_b1(f1_std_tran_b4(expr));
    }
    else if (base_num == 4)
    {
        return f1_b4_tran_b2(f1_std_tran_b4(expr));
    }
    else if (base_num == 8)
    {
        return f1_b4_tran_b3(f1_std_tran_b4(expr));
    }
    else if (base_num == 16)
    {
        return f1_std_tran_b4(expr);
    }
    else // base_num == 32
    {
        return f1_b4_tran_b5(f1_std_tran_b4(expr));
    }
}

void f3_renew_NDA(struct RegionTreeNode **regn_forest, int regn_tree_num, string addr)
{
    struct RegionTreeNode *regn_ptr;

    for (int i = 0; i < regn_tree_num; i++)
    {
        if (f3_expression_belong(addr, regn_forest[i]->expression))
        {
            regn_ptr = regn_forest[i];
            break;
        }
    }
    
    while (regn_ptr->is_leaf != true)
    {
        for (int i = 0; i < base_num; i++)
        {
            if (regn_ptr->children[i] != NULL)
            {
                if (f3_expression_belong(addr, regn_ptr->children[i]->expression))
                {
                    regn_ptr = regn_ptr->children[i];
                    break;
                }
            }
        }
    }

    struct PreparedSpaceTreeNode *spe_ptr = regn_ptr->spe_node_ptr;
    spe_ptr->NDA++;
}

int f3_density_cmp(struct SequenceNode *se_node1, struct SequenceNode *se_node2)
{
    // Compare the density: NDA_1/|SS_1| > NDA_2/|SS_2|

    int dimensionality = f2_get_dimensionality(base_num);
    
    struct PreparedSpaceTreeNode *spe_node = se_node1->node;
    int NDA = spe_node->NDA;
    int expr_num = spe_node->SS.num;
    int SS_scale = 0;
    for (int i = 0; i < expr_num; i++)
    {
        string expr = spe_node->SS.expressions[i];
        int star_num = 0;
        for (int j = 0; j < dimensionality; j++)
        {
            if (expr[j] == '*')
            {
                star_num++;
            }
        }
        SS_scale += pow(base_num, star_num);
    }
    double den1 = (double )NDA / (double )SS_scale;

    spe_node = se_node2->node;
    NDA = spe_node->NDA;
    expr_num = spe_node->SS.num;
    SS_scale = 0;
    for (int i = 0; i < expr_num; i++)
    {
        string expr = spe_node->SS.expressions[i];
        int star_num = 0;
        for (int j = 0; j < dimensionality; j++)
        {
            if (expr[j] == '*')
            {
                star_num++;
            }
        }
        SS_scale += pow(base_num, star_num);
    }
    double den2 = (double )NDA / (double )SS_scale;

    return den1 > den2;
}

struct SequenceNode *f3_local_feedback(ofstream &active_addrs, struct RegionTreeNode **regn_forest, int regn_tree_num, struct SequenceNode *xi_h)
{
    // 1. Renew TSs and SSs of space tree nodes in xi_h.
    struct SequenceNode *xi_ptr = xi_h;
    while (xi_ptr != NULL)
    {
        struct PreparedSpaceTreeNode *spe_node = xi_ptr->node;
        f3_copy_TS2SS(spe_node);
        int dimen = f3_DS_pop(spe_node);
        f3_TS_expand(spe_node, dimen);
        xi_ptr = xi_ptr->next;
    }

    // 2. Read active addresses from active_addrs, renew NDAs of space tree nodes in xi_h.
    string line;
    while (getline(active_addrs, line))
    {
        string addr = f3_std_tran_addr(line);
        f3_renew_NDA(regn_forest, regn_tree_num, addr);
    }

    // 3. Resort space tree nodes of xi_h, based on NDA/|SS|.
    int xi_len = 0;
    xi_ptr = xi_h;
    while (xi_ptr != NULL)
    {
        xi_len++;
        xi_ptr = xi_ptr->next;
    }
    struct SequenceNode **xi_arr = struct SequenceNode *[xi_len + 2];
    xi_ptr = xi_h;
    for (int i = 0; i < xi_len; i++)
    {
        xi_arr[i] = xi_ptr;
        xi_ptr = xi_ptr->next;
    }
    sort(xi_arr, xi_arr + xi_len, f3_density_cmp);
    for (int i = 0; i < xi_len - 1; i++)
    {
        xi_arr[i]->next = xi_arr[i + 1];
    }
    xi_arr[xi_len - 1]->next = NULL;
    struct SequenceNode *new_xi_h = xi_arr[0];
    delete [] xi_arr;
    return new_xi_h;
}

void f3_local_scan_feedback(struct SequenceNode *xi_h, int &budget, ofstream &addr_total_res, struct SearchTreeNode *A)
{
    // 1. Prepare the target address set C.
    
    // Count the number of TSs.
    int TS_num = 0;
    struct SequenceNode *xi_ptr = xi_h;
    while (xi_ptr != NULL)
    {
        TS_num += xi_ptr->node->TS.num;
        xi_ptr = xi_ptr->next;
    }
    
    // Generate a corresponding region tree forest base on the data, for representing the targets.
    // The index starts at 0.
    struct RegionTreeNode **regn_forest = new struct RegionTreeNode *[TS_num + 2];
    xi_ptr = xi_h;
    int forest_idx = 0;
    while (xi_ptr != NULL)
    {
        f3_init_region_trees(regn_forest, forest_idx, xi_ptr);
        xi_ptr = xi_ptr->next;
    }
    
    // 2. Perform the scan and renew budget and addr_total_res.
    
    ofstream active_addrs_write;
    active_addrs.open(_STEP_RES_FILE);
    f3_local_scan(regn_forest, TS_num, budget, addr_total_res, active_addrs_write, A);
    active_addrs_write.close();
    
    // 3. Renew information of nodes in xi_h, and resort xi_h.

    ifstream active_addrs_read;
    active_addrs_read.open(_STEP_RES_FILE);
    struct SequenceNode *new_xi_h = f3_local_feedback(active_addrs_read, regn_forest, TS_num, xi_h);
    xi_h = new_xi_h;
    active_addrs_read.close();
    
    // Remove the file corresponding to active_addrs.
    remove(_STEP_RES_FILE);
    
    // Release regn_forest.
    for (int i = 0; i < TS_num; i++)
    {
        f3_release_region_tree(regn_forest[i]);
    }
    delete [] regn_forest;
}

int f3_calcscale_subset_TSSS(struct PreparedSpaceTreeNode *spe_ptr)
{
    int dimensionality = f2_get_dimensionality(base_num);

    int addrnum_TS = 0;
    int expr_num = spe_ptr->TS.num;
    for (int i = 0; i < expr_num; i++)
    {
        string expr = spe_ptr->TS.expressions[i];
        int star_num = 0;
        for (int j = 0; j < dimensionality; j++)
        {
            if (expr[j] == '*')
            {
                star_num++;
            }
        }
        addrnum_TS += pow(base_num, star_num);
    }

    int addrnum_SS = 0;
    expr_num = spe_ptr->SS.num;
    for (int i = 0; i < expr_num; i++)
    {
        string expr = spe_ptr->SS.expressions[i];
        int star_num = 0;
        for (int j = 0; j < dimensionality; j++)
        {
            if (expr[j] == '*')
            {
                star_num++;
            }
        }
        addrnum_SS += pow(base_num, star_num);
    }

    return addrnum_TS - addrnum_SS;
}

struct SequenceNode *f3_cut_fseg(struct SequenceNode *&xi, int itn_budget)
{
    // Select anterior nodes from xi, based on itn_budget.

    struct SequenceNode *xi_h = xi;
    struct SequenceNode *xi_ptr = xi;
    while (true)
    {
        if (xi_ptr == NULL)
        {
            xi = NULL;
            return xi_h;
        }

        struct PreparedSpaceTreeNode *spe_ptr =  xi_ptr->node;
        itn_budget -= f3_calc_subset_TSSS(spe_ptr);
        if (itn_budget > 0)
        {
            xi_ptr = xi_ptr->next;
        }
        else
        {
            xi = xi_ptr->next;
            xi_ptr->next = NULL;
            return xi_h;
        }
    }
}

bool f3_same_DS(struct PreparedSpaceTreeNode *ptr1, struct PreparedSpaceTreeNode *ptr2)
{
    // Return true if the DSs are same, else return false.

    if (ptr1->DS.num != ptr2->DS.num)
    {
        return false;
    }
    int DS_num = ptr1->DS.num;
    for (int i = 0; i < DS_num; i++)
    {
        if (ptr1->DS.stack[i] != ptr2->DS.stack[i])
        {
            return false;
        }
    }
    return true;
}

void f3_copy_TS2parent(struct PreparedSpaceTreeNode *ptr, struct PreparedSpaceTreeNode *pptr)
{
    int TS_num = ptr->TS.num;
    pptr->TS.num = TS_num;
    pptr->TS.expressions = new string [TS_num];
    for (int i = 0; i < TS_num; i++)
    {
        pptr->TS.expressions[i] = ptr->TS.expressions[i];
    }
}

bool f3_is_descendant(struct PreparedSpaceTreeNode *ptr, struct PreparedSpaceTreeNode *pptr)
{
    // Check if ptr is descendant node of pptr, based on: if SS of ptr is included by TS of pptr.

    string ptr_expr = ptr->SS.expressions[0];
    int TS_num = pptr->TS.num;
    for (int i = 0; i < TS_num; i++)
    {
        if (f3_expression_belong(ptr_expr, pptr->TS.expressions[i]))
        {
            return true;
        }
    }
    return false;
}

struct SequenceNode *f3_get_retired_nodes(struct SequenceNode *xi, struct SequenceNode *xi_h, struct PreparedSpaceTreeNode *node_ptr)
{
    // Get the intersection set between descendant nodes of node_ptr and (xi cup xi_h), return the result as a linked list.

    struct SequenceNode *retired_nodes = NULL;
    struct SequenceNode *retired_nodes_tail = NULL;

    struct SequenceNode *xi_ptr = xi;
    while (xi_ptr != NULL)
    {
        if (f3_is_descendant(xi_ptr->node, node_ptr))
        {
            // Add xi_ptr->node into retired_nodes.
            if (retired_nodes == NULL)
            {
                retired_nodes = new struct SequenceNode;
                retired_nodes->node = xi_ptr->node;
                retired_nodes->next = NULL;
                retired_nodes_tail = retired_nodes;
            }
            else
            {
                struct SequenceNode *new_node = new struct SequenceNode;
                new_node->node = xi_ptr->node;
                new_node->next = NULL;
                retired_nodes_tail->next = new_node;
                retired_nodes_tail = new_node;
            }
        }
        xi_ptr = xi_ptr->next;
    }

    struct SequenceNode *xi_h_ptr = xi_h;
    while (xi_h_ptr != NULL)
    {
        if (f3_is_descendant(xi_h_ptr->node, node_ptr))
        {
            // Add xi_h_ptr->node into retired_nodes;
            if (retired_nodes == NULL)
            {
                retired_nodes = new struct SequenceNode;
                retired_nodes->node = xi_h_ptr->node;
                retired_nodes->next = NULL;
                retired_nodes_tail = retired_nodes;
            }
            else
            {
                struct SequenceNode *new_node = new struct SequenceNode;
                new_node->node = xi_h_ptr->node;
                new_node->next = NULL;
                retired_nodes_tail->next = new_node;
                retired_nodes_tail = new_node;
            }

        }
        xi_h_ptr = xi_h_ptr->next;
    }

    return retired_nodes;
}

struct VectorRegion f3_gather_descendant_SS(struct SequenceNode *retired_nodes)
{
    struct VectorRegion SS;

    int SS_num = 0;
    struct SequenceNode *ptr = retired_nodes;
    while (ptr != NULL)
    {
        SS_num += ptr->node->SS.num;
        ptr = ptr->next;
    }
    SS.num = SS_num;

    SS.expressions = new string [SS_num];
    int i = 0;
    ptr = retired_nodes;
    while (ptr != NULL)
    {
        for (int j = 0; j < ptr->node->SS.num; j++)
        {
            SS.expressions[i++] = ptr->node->SS.expressions[j];
        }
        ptr = ptr->next;
    }

    return SS;
}

int f3_gather_descendant_NDA(struct SequenceNode *retired_nodes)
{
    int NDA = 0;

    struct SequenceNode *ptr = retired_nodes;
    while (ptr != NULL)
    {
        NDA += ptr->node->NDA;
        ptr = ptr->next;
    }

    return NDA;
}

void f3_replace_descendant(struct SequenceNode *&xi, struct SequenceNode *&xi_h)
{
    struct SequenceNode *new_nodes = NULL;
    struct SequenceNode *new_nodes_tail = NULL;

    // 1. Add necessary parent nodes into new_nodes.
    struct SequenceNode *xi_h_ptr = xi_h;
    while (xi_h_ptr != NULL)
    {
        struct PreparedSpaceTreeNode *spe_ptr = xi_h_ptr->node;
        struct PreparedSpaceTreeNode *spe_pptr = spe_ptr->parent;
        if (spe_pptr == NULL)
        {
            // spe_ptr is the root node, in the case, there is only one node in xi cup xi_h.
            continue;
        }
        if (f3_same_DS(spe_pptr, spe_ptr))
        {
            // Copy the TS information to the parent node.
            f3_copy_TS2parent(spe_ptr, spe_pptr);
            // Add the parent node into new_nodes.
            if (new_nodes == NULL)
            {
                new_nodes = new struct SequenceNode;
                new_nodes->node = spe_pptr;
                new_nodes->next = NULL;
                new_nodes_tail = new_nodes;
            }
            else
            {
                struct SequenceNode *new_node = new struct SequenceNode;
                new_node->node = spe_pptr;
                new_node->next = NULL;
                new_nodes_tail->next = new_node;
                new_nodes_tail = new_node;
            }
        }
        xi_h_ptr = xi_h_ptr->next;
    }

    // 2. Perform the replacement in xi and xi_h.
    if (new_nodes == NULL)
    {
        return ;
    }

    // Generate an array to store information of new_nodes.
    int new_nodes_arr_scale = 0;
    struct SequenceNode *new_nodes_ptr = new_nodes;
    while (new_nodes_ptr != NULL)
    {
        new_nodes_arr_scale++;
        new_nodes_ptr = new_nodes_ptr->next;
    }
    struct PreparedSpaceTreeNode **new_nodes_arr = new struct PreparedSpaceTreeNode *[new_nodes_arr_scale + 2];
    new_nodes_ptr = new_nodes;
    for (int i = 0; i < new_nodes_arr_scale; i++)
    {
        new_nodes_arr[i] = new_nodes_ptr->node;
        new_nodes_ptr = new_nodes_ptr->next;
    }

    for (int i = 0; i < new_nodes_arr_scale; i++)
    {
        struct SequenceNode *retired_nodes = f3_get_retired_nodes(xi, xi_h, new_nodes_arr[i]);
        new_nodes_arr[i]->SS = f3_gather_descendant_SS(retired_nodes);
        new_nodes_arr[i]->NDA = f3_gather_descendant_NDA(retired_nodes);
        // 分别在xi, xi_h, new_nodes_arr中去除存在于retired_nodes中的空间树结点
        struct SequenceNode *new_seq = f3_delete_retired_inseq(xi, retired_nodes); // -- need work: 写到了这里
    }
    // 注意new_nodes, retired_nodes, arr最后要释放才行
    // 注意xi_h肯定不会是空集，但是xi可能是空集
}

void f3_work(int type1, string str2, int type3, string str4, int type5, string str6)
{
    // 1. Analyze instructions.
    string treedir_name;
    string testfile_name;
    string outres_name;
    int test_type;
    if (type1 == _INS_INTREE)
    {
        treedir_name = str2;
    }
    else if (type3 == _INS_INTREE)
    {
        treedir_name = str4;
    }
    else // type5 == _INS_INTREE
    {
        treedir_name = str6;
    }
    if (type1 >= _INS_TESTSTD && type1 <= _INS_TESTB5)
    {
        testfile_name = str2;
        test_type = type1;
    }
    else if (type3 >= _INS_TESTSTD && type3 <= _INS_TESTB5)
    {
        testfile_name = str4;
        test_type = type3;
    }
    else // type5 >= _INS_TESTSTD && type5 <= _INS_TESTB5
    {
        testfile_name = str6;
        test_type = type5;
    }
    if (type1 == _INS_OUTRES)
    {
        outres_name = str2;
    }
    else if (type3 == _INS_OUTRES)
    {
        outres_name = str4;
    }
    else // type5 == _INS_OUTRES
    {
        outres_name = str6;
    }
    
    f1_print_time();
    cout << "[Local test] Start local test." << endl;
    
    // 2. Input data.
    
    // 2.1 Configure the space tree data.
    f1_print_time();
    cout << "[Local test] Prepare space tree." << endl;
    
    // Generate space tree.
    struct PreparedSpaceTreeNode *root = f3_prepare_space_tree(treedir_name);
    // Generate leaf node sequence.
    struct SequenceNode *xi = f3_gather_leaves(root, NULL);
    
    f1_print_time();
    cout << "[Local test] Prepare space tree finished." << endl;
    
    // 2.2 Configure the information of test addresses which are hypothetical all active addresses.
    f1_print_time();
    cout << "[Local test] Prepare search tree." << endl;
    
    struct SearchTreeNode *A = f3_prepare_test_data(testfile_name, test_type, treedir_name);
    
    f1_print_time();
    cout << "[Local test] Prepare search tree finished." << endl;
    
    // 3. Run local simulation search.
    
    // 3.1 Configure the scan parameters.
    int budget, itn_budget;
    f3_read_local_scan_parameters(budget, itn_budget, treedir_name);
    
    // 3.2 Pre-scanning.
    string res_dir_str = "./" + outres_name;
    mkdir((const char*)(res_dir_str.c_str()), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    // addr_res stores all found active addresses.
    ofstream addr_total_res;
    addr_total_res.open(res_dir_str + "/" + _RES_FILE);
    f3_local_scan_feedback(xi, budget, addr_total_res, A);
    
    // 3.3 Iterative scanning, until using out the budget.
    while (budget > 0)
    {
        // There is no alias measurement in the local test.

        // 3.3.1 Select anterior nodes from xi, based on itn_budget.
        struct SequenceNode *xi_h = f3_cut_fseg(xi, itn_budget);

        // 3.3.2 Perform the node replacement.
        f3_replace_descendant(xi, xi_h);

        // ----

        // -- need work: 写到了这里，答辩结束后继续写

        // 首先就要根据itn_budget来截取出xi_h

        // 然后检查xi_h中的结点是否有要被取代的，进行取代工作

        // 进行扫描和反馈

        // 进行归并排序
    }

    // -- need work: 最后扫描完了之后可以检查一下总的活跃地址发现结果文件中是否有重复地址，原则上讲应该不会。
    // 3.4 Output results. 那么这个地方addr_total_res应该已经输出完了，看看Iris信息怎么输出。
    ofstream iris_res;
    
    // 3.5 Release data.
    // -- need work: 生成的空间树，搜索树，结点指针序列最后需要释放。
    addr_total_res.close();
    
    f1_print_time();
    cout << "[Local test] Local test finished." << endl;
}

void f3_access(int argc, const char * argv[])
{
    // 6tree -L (-in-tree *tree folder name*) (-test-std/b* *test address file name*) (-out-res *result folder name*)
    
    // 1. *test address file* store addresses which are hypothetical all active addresses. Must be sorted and unique.
    // 2. The base modes include binary (b2), quatenary (b2), ..., base-32 (b5). b3 omits the first two bits (00), b5 omits the first three bits (001).
    // 3. Local test doesn't include the alias detection.
    
    if (argc != 8)
    {
        cout << "[Error] Function instruction is incorrect." << endl;
        return;
    }
    
    // Check instruction correctness.
    int type1 = f1_type_ins(string(argv[2]));
    int type3 = f1_type_ins(string(argv[4]));
    int type5 = f1_type_ins(string(argv[6]));
    
    int pool_ins_dir = 0;
    int pool_ins_test = 0;
    int pool_ins_out = 0;
    
    if (type1 == _INS_INTREE)
    {
        pool_ins_dir++;
    }
    if (type3 == _INS_INTREE)
    {
        pool_ins_dir++;
    }
    if (type5 == _INS_INTREE)
    {
        pool_ins_dir++;
    }
    if (type1 >= _INS_TESTSTD && type1 <= _INS_TESTB5)
    {
        pool_ins_test++;
    }
    if (type3 >= _INS_TESTSTD && type3 <= _INS_TESTB5)
    {
        pool_ins_test++;
    }
    if (type5 >= _INS_TESTSTD && type5 <= _INS_TESTB5)
    {
        pool_ins_test++;
    }
    if (type1 == _INS_OUTRES)
    {
        pool_ins_out++;
    }
    if (type3 == _INS_OUTRES)
    {
        pool_ins_out++;
    }
    if (type5 == _INS_OUTRES)
    {
        pool_ins_out++;
    }
    if (pool_ins_dir != 1 || pool_ins_test != 1 || pool_ins_out != 1)
    {
        cout << "[Error] Function instruction is incorrect." << endl;
        return;
    }
    
    // Start the local test.
    f3_work(type1, string(argv[3]), type3, string(argv[5]), type5, string(argv[7]));
}
