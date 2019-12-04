//
//  function3_L.hpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/3/2.
//

#ifndef function3_L_hpp
#define function3_L_hpp

using namespace std;

void f3_trim(string &s);

void f3_initialize_DS(struct PreparedSpaceTreeNode *node, struct DimenStack *parent_DS, int dimensionality, string *arr);

int f3_DS_pop(struct PreparedSpaceTreeNode *node);

void f3_TS_expand(struct PreparedSpaceTreeNode *node, int dimen);

void f3_initialize_TS_SS_NDA(struct PreparedSpaceTreeNode **node_arr, int node_num, string *arr);

struct PreparedSpaceTreeNode *f3_prepare_space_tree(string treedir_name);

struct SequenceNode *f3_gather_leaves(struct PreparedSpaceTreeNode *node, struct SequenceNode *link_node);

int f3_get_type(int base_num);

struct SearchTreeNode *f3_generate_search_tree(string *arr, int arr_scale);

int f3_testtype_tran_intype(int test_type);

struct SearchTreeNode *f3_prepare_test_data(string testfile_name, int test_type, string treedir_name);

void f3_read_local_scan_parameters(int &budget, int &itn_budget, string treedir_name);

void f3_release_region_tree(struct RegionTreeNode *regn_ptr);

void f3_region_tree_subtract(struct RegionTreeNode *regn_root, string expr);

bool f3_expression_belong(string str1, string str2);

void f3_init_region_trees(struct RegionTreeNode **region_forest, int &forest_idx, struct SequenceNode *xi_ptr);

string f3_addr_tran_std(string expr);

void f3_local_rec_output_addrs(struct SearchTreeNode *sch_node, ofstream &addr_total_res, ofstream &active_addrs);

int f3_local_rec_search(string expr, struct SearchTreeNode *sch_node, ofstream &addr_total_res, ofstream &active_addrs);

int f3_scan_on_leaves(struct RegionTreeNode *regn_node, struct SearchTreeNode *A, ofstream &addr_total_res, ofstream &active_addrs);

void f3_local_scan(struct RegionTreeNode **regn_forest, int tree_num, int &budget, ofstream &addr_total_res, ofstream &active_addrs, struct SearchTreeNode *A);

void f3_copy_TS2SS(struct PreparedSpaceTreeNode *spe_node);

string f3_std_tran_addr(string expr);

void f3_renew_NDA(struct RegionTreeNode **regn_forest, int regn_tree_num, string addr);

int f3_density_cmp(struct SequenceNode se_node1, struct SequenceNode se_node2);

struct SequenceNode *f3_local_feedback(ofstream &active_addrs, struct RegionTreeNode **regn_forest, int regn_tree_num, struct SequenceNode *xi_h);

void f3_local_scan_feedback(struct SequenceNode *xi_h, int &budget, ofstream &addr_total_res, struct SearchTreeNode *A);

void f3_work(int type1, string str2, int type3, string str4, int type5, string str6);

void f3_access(int argc, const char * argv[]);

#endif /* function3_L_hpp */
