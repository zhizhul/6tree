//
//  function4_R.cpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/12/12.
//

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

struct SequenceNode *f4_network_feedback(struct RegionTreeNode **regn_forest, int regn_tree_num, struct SequenceNode *xi_h, int active_addr_num)
{
    // 1. Renew TSs and SSs of space tree nodes in xi_h.
    struct SequenceNode *xi_ptr = xi_h;
    while (xi_ptr != NULL)
    {
        struct PreparedSpaceTreeNode *spe_node = xi_ptr->node;
        f3_copy_TS2SS(spe_node);
        int dimension = f3_DS_pop(spe_node);
        f3_TS_expand(spe_node, dimension);
        xi_ptr = xi_ptr->next;
    }

    // 2. Read active addresses from active_addrs_read, renew NDAs of sapce tree nodes in xi_h.

    // 2.1 Get detected active addresses.
    // In default, the sort has been performed in si_network_scan() from scanner_interface.cpp.
    string *arr = new string [active_addr_num + 2];
    ifstream active_addrs_read;
    active_addrs_read.open(_SI_STEP_RES_FILE);
    string line;
    int arr_idx = 0;
    while (getline(active_addrs_read, line))
    {
        arr[arr_idx++] = f3_std_tran_addr(line);
    }
    active_addrs_read.close();

    // 2.2 Sort region trees based on their region expressions.
    sort(regn_forest, regn_forest, regn_tree_num, f3_regn_cmp);

    // 2.3 Perform the renew operation.
    f3_renew_NDA(regn_forest, regn_tree_num, arr, active_addr_num);
    delete [] arr;

    // 3. Resort space tree nodes of xi_h, based on NDA/|SS|.
    int xi_len = 0;
    xi_ptr = xi_h;
    while (xi_ptr != NULL)
    {
        xi_len++;
        xi_ptr = xi_ptr->next;
    }
    struct SequenceNode **xi_arr = new struct SequenceNode *[xi_len + 2];
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

int f4_network_scan_feedback(struct SequenceNode *&xi_h, int &budget, ofstream &addr_total_res, ofstream &scan_log)
{
    // 1. Prepare the target address set C, represented as a region tree forest.

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
    struct RegionTreeNode **regn_forest = new struct RegionTreeNode *[TS_num];
    xi_ptr = xi_h;
    int forest_idx = 0;
    while (xi_ptr != NULL)
    {
        f3_init_region_trees(regn_forest, forest_idx, xi_ptr);
        xi_ptr = xi_ptr->next;
    }
    
    // 2. Perform the scan and renew budget and addr_total_res.
    
    int active_addr_num = si_network_scan(regn_forest, TS_num, budget, addr_total_res);

    // 3. Renew information of nodes in xi_h, and resort xi_h.

    struct SequenceNode *new_xi_h = f4_network_feedback(regn_forest, TS_num, xi_h, active_addr_num);
    xi_h = new_xi_h;

    f1_print_time();
    cout << endl;
    cout << "find active addresses: " << active_addr_num << ", budget remains: " << budget << endl;
    f3_print_time(scan_log);
    scan_log << endl;
    scan_log << "find active addresses: " << active_addr_num << ", budget remains: " << budget << endl;

    // 4. Epilogue.
    
    // Remove the file corresponding to _SI_STEP_RES_FILE.
    remove(_SI_STEP_RES_FILE);
    // Remove the file corresponding to _SI_STEP_TF_FILE.
    remove(_SI_STEP_TF_FILE);

    // Release regn_forest.
    for (int i = 0; i < TS_num; i++)
    {
        f3_release_region_tree(regn_forest[i]);
    }
    delete [] regn_forest;

    return active_addr_num;
}

void f4_work(int type1, string str2, int type3, string str4)
{
    // 1. Analyze instructions.
    string treedir_name;
    string outres_name;

    if (type1 == _INS_INTREE) treedir_name = str2;
    else treedir_name = str4; // type3 == _INS_INTREE
    if (type1 == _INS_OUTRES) outres_name = str2;
    else outres_name = str4; // type3 == _INS_OUTRES

    f1_print_time();
    cout << "[Network search] Start network search." << endl;

    // 2. Input data.

    // 2.1 Configure the space tree data.
    f1_print_time();
    cout << "[Network search] Prepare space tree." << endl;

    // Generate space tree.
    struct PreparedSpaceTreeNode *root = f3_prepare_space_tree(treedir_name);
    // Generate leaf node sequence.
    struct SequenceNode *xi = f3_gather_leaves(root, NULL);

    f1_print_time();
    cout << "[Network search] Prepare space tree finished." << endl;

    // 2.2 Configure the scanner parameters, i.e., the command line parameters.
    f1_print_time();
    cout << "[Network search] Read scanner parameters." << endl;

    si_read_scanner_command(treedir_name);

    f1_print_time();
    cout << "[Network search] Read scanner parameters finished." << endl;

    // 3. Run active address search on the net.

    // 3.1 Configure the budget parameters.
    int budget, itn_budget;
    f3_read_budget_parameters(budget, itn_budget, treedir_name);

    // 3.2 Pre-scanning.
    string res_dir_str = "./" + outres_name;
    mkdir((const char*)(res_dir_str.c_str()), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   
    // addr_res stores all found active addresses.
    ofstream addr_total_res;
    int addr_total_num = 0;
    addr_total_res.open(res_dir_str + "/" + _RES_FILE);
    ofstream scan_log;
    scan_log.open(res_dis_str + "/" + _LOG_FILE);
    addr_total_num += f4_network_scan_feedback(xi, budget, addr_total_res, scan_log);

    f1_print_time();
    cout << endl << "Pre scanning finished." << endl;
    f3_print_time(scan_log);
    scan_log << endl << "Pre scanning finished." << endl;

    // 3.3 Iterative scanning, until using out the budget.
    while (budget > 0)
    {
        // 3.3.1 Select anterior nodes from xi, based on itn_budget.
        struct SequenceNode *xi_h = f3_cut_fseg(xi, itn_budget);

        // 3.3.2 Perform the node replacement.
        f3_replace_descendant(xi, xi_h);

        // 3.3.3 Alias detection.
        // -- need work
        f4_alias_detection(xi, xi_h, budget, addr_total_res, scan_log);

        // 3.3.4 Scan and feedback.
        addr_total_num += f4_network_scan_feedback(xi_h, budget, addr_total_res, scan_log);

        // 3.3.5 Merge sort.
        struct SequenceNode *tptr = f3_mergesort(xi, xi_h);
        xi = tptr;
    }

    f1_print_time();
    cout << endl << "Total scanning finished." << endl;
    cout << "find total active addresses: " << addr_total_num << endl;
    f3_print_time(scan_log);
    scan_log << endl << "Total scanning finished." << endl;
    scan_log << "find total active addresses: " << addr_total_num << endl;
    addr_total_res.close();
    scan_log.close();

    // 3.4 Output iris information.
    f1_print_time();
    cout << "[Network search] Output visualization information." << endl;
    int node_num = f3_clear_NDA(root);
    ifstream addr_total_res_read;
    addr_total_res_read.open(res_dir_str + "/" + _RES_FILE);
    f3_count_NDA(addr_total_res_read, root);
    addr_total_res_read.close();
    ofstream iris_res;
    iris_res.open(res_dir_str + "/" + _IRIS_FILE);
    f3_output_iris(iris_res, node_num, root);
    iris_res.close();
    f1_print_time();
    cout << "[Network search] Output visualization information finished." << endl;

    // 3.5 Release space tree and sequence data.
    f1_print_time();
    

    f1_print_time();
    cout << "[Network search] Network search finished." << endl;
}

void f4_access(int argc, const char * argv[])
{
    // 6tree -R (-in-tree *tree folder name*) (-out-res *result folder name*)

    if (argc != 6)
    {
        cout << "[Error] Function instruction is incorrect." << endl;
        return;
    }

    // Check instruction correctness.
    int type1 = f1_type_ins(string(argv[2]));
    int type3 = f1_type_ins(string(argv[4]));

    int pool_ins_dir = 0;
    int pool_ins_out = 0;

    if (type1 == _INS_INTREE) pool_ins_dir++;
    if (type3 == _INS_INTREE) pool_ins_dir++;
    if (type1 == _INS_OUTRES) pool_ins_out++;
    if (type3 == _INS_OUTRES) pool_ins_out++;

    if (pool_ins_dir != 1 || pool_ins_out != 1)
    {
        cout << "[Error] Function instruction is incorrect." << endl;
        return;
    }

    // Start the real scan.
    f4_work(type1, string(argv[3]), type3, string(argv[5]));
}
