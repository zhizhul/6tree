//
//  function2_G.hpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/12/10.
//

#ifndef function2_G_hpp
#define function2_G_hpp

using namespace std;

int f2_get_base_num(int in_type);

int f2_get_dimensionality(int base_num);

void f2_DHC(int base_num, struct SpaceTreeNode *node, string *arr, int &node_num);

struct SpaceTreeNode *f2_tree_generation(int in_type, string *arr, int arr_scale, int &node_num);

string f2_to_lower(string line, int dimensionality);

void f2_release_tree(struct SpaceTreeNode *node);

void f2_store_node(struct SpaceTreeNode **node_arr, int &node_arr_scale, struct SpaceTreeNode *node);

void f2_output_tree(struct SpaceTreeNode *root, int node_num, int in_type, string outdir_name);

void f2_output_arr(string *arr, int arr_scale, string outdir_name);

void f2_output_scan_parameter(string outdir_name);

void f2_work(int type1, string str2, int type3, string str4);

void f2_access(int argc, const char * argv[]);

#endif /* function2_G_hpp */
