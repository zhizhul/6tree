//
//  function4_R.hpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/12/12.
//

#ifndef function4_R_hpp
#define function4_R_hpp

using namespace std;

struct SequenceNode *f4_network_feedback(struct RegionTreeNode **regn_forest, int regn_tree_num, struct SequenceNode *xi_h);

int f4_network_scan_feedback(struct SequenceNode *&xi_h, int &budget, ofstream &addr_total_res, ofstream &scan_log);

void f4_work(int type1, string str2, int type3, string str4);

void f4_access(int argc, const char *argv[]);

#endif /* function4_R_hpp */
