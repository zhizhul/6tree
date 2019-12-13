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

    si_read_scanner_parameters(treedir_name);

    f1_print_time();
    cout << "[Network scan] Read scanner parameters finished." << endl;

    // 3. Run active address search on the net.

    // 3.1 Configure the budget parameters.

    // 3.2 Pre-scanning.

    // 3.3 Iterative scanning, until using out the budget.

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
