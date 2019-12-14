//
//  function4_R.hpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/12/12.
//

#ifndef function4_R_hpp
#define function4_R_hpp

using namespace std;

struct SequenceNode *f4_network_feedback(struct RegionTreeNode **regn_forest, int regn_tree_num, struct SequenceNode *xi_h, int active_addr_num);

int f4_network_scan_feedback(struct SequenceNode *&xi_h, int &budget, ofstream &addr_total_res, ofstream &scan_log);

double f4_calc_thd(struct PreparedSpaceTreeNode *node, double adet_zeta, double adet_pi, int dimensionality);

bool f4_is_potential(struct PreparedSpaceTreeNode *node, double adet_zeta, double adet_pi, int dimensionality);

struct SequenceNode *f4_takeout_pnode(struct SequenceNode *&xi_h, double adet_zeta, double adet_pi);

void f4_pnode_analysis(struct SequenceNode *pnode, struct SequenceNode *&xi, struct SequenceNode *&xi_h, int &budget, struct AdetParameters adet_ps, ofstream &addr_total_res, int &addr_total_num, ofstream &scan_log, ofstream &ali_file);

void f4_alias_detection(struct SequenceNode *&xi, struct SequenceNode *&xi_h, int &budget, struct AdetParameters adet_ps, ofstream &addr_total_res, int &addr_total_num, ofstream &scan_log, ofstream &ali_file);

void f4_read_search_parameters(int &budget, int &itn_budget, struct AdetParameters &adet_ps, string treedir_name);

void f4_work(int type1, string str2, int type3, string str4);

void f4_access(int argc, const char *argv[]);

#endif /* function4_R_hpp */
