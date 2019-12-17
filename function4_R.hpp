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

int f4_calc_TSscale(struct PreparedSpaceTreeNode *node);

bool f4_is_potential(struct PreparedSpaceTreeNode *node, double adet_zeta, double adet_pi, int dimensionality);

struct SequenceNode *f4_takeout_pnode(struct SequenceNode *&xi_h, double adet_zeta, double adet_pi);

string *f4_psurdgen_targets(struct VectorRegion TS, int l_dimension, int ptimes, int &targets_num);

int f4_get_lastdimension(struct DimenStack DS);

int f4_adet_scan_feedback(string *targets, int targets_num, int &budget, ofstream &scan_log);

void f4_adet_replace_descendant(struct SequenceNode *&pnode, struct SequenceNode *&xi, struct SequenceNode *&xi_h);

void f4_insert(struct SequenceNode *&xi, struct SequenceNode *pnode);

void f4_set_aliased(struct PreparedSpaceTreeNode *ptr);

int f4_pnode_analysis(struct SequenceNode *pnode, struct SequenceNode *&xi, struct SequenceNode *&xi_h, int &budget, struct AdetParameters adet_ps, ofstream &addr_total_res, ofstream &scan_log, ofstream &ali_file);

int f4_alias_detection(struct SequenceNode *&xi, struct SequenceNode *&xi_h, int &budget, struct AdetParameters adet_ps, ofstream &addr_total_res, ofstream &scan_log, ofstream &ali_file);

void f4_read_search_parameters(int &budget, int &itn_budget, struct AdetParameters &adet_ps, string treedir_name);

void f4_output_iris(ofstream &iris_res, int node_num, struct PreparedSpaceTreeNode *root);

void f4_work(int type1, string str2, int type3, string str4);

void f4_access(int argc, const char *argv[]);

#endif /* function4_R_hpp */
