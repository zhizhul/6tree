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
    
    si_network_scan(regn_forest, TS_num, budget, addr_total_res);

    // 3. Renew information of nodes in xi_h, and resort xi_h.
    // -- need work

    // 4. Epilogue.
    // -- need work
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
    cout << "[Network scan] Start network scan." << endl;

    // 2. Input data.

    // 2.1 Configure the space tree data.
    f1_print_time();
    cout << "[Network scan] Prepare space tree." << endl;

    // Generate space tree.
    struct PreparedSpaceTreeNode *root = f3_prepare_space_tree(treedir_name);
    // Generate leaf node sequence.
    struct SequenceNode *xi = f3_gather_leaves(root, NULL);

    f1_print_time();
    cout << "[Network scan] Prepare space tree finished." << endl;

    // 2.2 Configure the scanner parameters, i.e., the command line parameters.
    f1_print_time();
    cout << "[Network scan] Read scanner parameters." << endl;

    si_read_scanner_command(treedir_name);

    f1_print_time();
    cout << "[Network scan] Read scanner parameters finished." << endl;

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

    addr_total_res.close();

    // 3.4 Output iris information.

    // 3.5 Release space tree and sequence data.

    f1_print_time();
    cout << "[Network scan] Network scan finished." << endl;
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
