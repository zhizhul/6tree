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
#include <cstdlib>
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
    // In default, the sort has been performed in scanner_interface.cpp=>si_network_scan().
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
    sort(regn_forest, regn_forest + regn_tree_num, f3_regn_cmp);

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
    if (xi_h == NULL)
    {
        int active_addr_num = 0;
        return active_addr_num;
    }

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
    cout << "Find active addresses: " << active_addr_num << ", budget remains: " << budget << endl;
    f3_print_time(scan_log);
    scan_log << endl;
    scan_log << "Find active addresses: " << active_addr_num << ", budget remains: " << budget << endl;

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

double f4_calc_thd(struct PreparedSpaceTreeNode *node, double adet_zeta, double adet_pi, int dimensionality)
{
    int star_num = 0;
    string expr = node->TS.expressions[0];
    for (int i = 0; i < dimensionality; i++)
    {
        if (expr[i] == '*')
        {
            star_num++;
        }
    }
    double numerator = adet_pi * log(adet_zeta) * (128.0 * log(2.0) - (double )star_num * log((double )base_num));
    double denominator = (128.0 * log(2.0) - log(adet_zeta)) * (double )star_num * log((double )base_num);
    double thd = numerator / denominator;
    return thd;
}

int f4_calc_TSscale(struct PreparedSpaceTreeNode *node)
{
    int TS_num = node->TS.num;
    string TS_expr0 = node->TS.expressions[0];
    int dimensionality = f2_get_dimensionality(base_num);
    int star_num = 0;
    for (int i = 0; i < dimensionality; i++)
    {
        if (TS_expr0[i] == '*')
        {
            star_num++;
        }
    }
    int scale = TS_num * (int )pow((double )base_num, (double )star_num);
    return scale;
}

bool f4_is_potential(struct PreparedSpaceTreeNode *node, double adet_zeta, double adet_pi, int dimensionality)
{
    if ((double )f4_calc_TSscale(node) < adet_zeta)
    {
        return false;
    }
    
    double AAD = f3_calc_density(node);
    double thd = f4_calc_thd(node, adet_zeta, adet_pi, dimensionality);
    if (AAD > thd)
    {
        return true;
    }
    else
    {
        return false;
    }
}

struct SequenceNode *f4_takeout_pnode(struct SequenceNode *&xi_h, double adet_zeta, double adet_pi)
{
    struct SequenceNode *ptr = xi_h;
    int dimensionality = f2_get_dimensionality(base_num);
    if (f4_is_potential(ptr->node, adet_zeta, adet_pi, dimensionality))
    {
        xi_h = xi_h->next;
        return ptr;
    }

    struct SequenceNode *ptr_prev = xi_h;
    ptr = ptr->next;
    while (ptr != NULL)
    {
        if (f4_is_potential(ptr->node, adet_zeta, adet_pi, dimensionality))
        {
            ptr_prev->next = ptr->next;
            return ptr;
        }

        ptr_prev = ptr;
        ptr = ptr->next;
    }
    return ptr;
}

string *f4_psurdgen_targets(struct VectorRegion TS, int l_dimension, int ptimes, int &targets_num)
{
    targets_num = base_num * ptimes * TS.num;
    string *arr = new string [targets_num + 2];
    int dimensionality = f2_get_dimensionality(base_num);
    int arr_idx = 0;
    for (int i = 0; i < TS.num; i++)
    {
        string background_str = TS.expressions[i];
        for (int j = 0; j < ptimes; j++)
        {
            string str = background_str;
            for (int k = 0; k < base_num; k++)
            {
                arr[arr_idx] = str;
                if (k < 10)
                {
                    arr[arr_idx][l_dimension] = '0' + k;
                }
                else // k >= 10
                {
                    arr[arr_idx][l_dimension] = 'a' + k - 10;
                }
                for (int t = 0; t < dimensionality; t++)
                {
                    if (arr[arr_idx][t] == '*')
                    {
                        int rd = rand() % base_num;
                        if (rd < 10)
                        {
                            arr[arr_idx][t] = '0' + rd;
                        }
                        else // rd >= 10
                        {
                            arr[arr_idx][t] = 'a' + rd - 10;
                        }
                    }
                }
                arr_idx++;
            }
        }
    }
    return arr;
}

int f4_get_lastdimension(struct DimenStack DS)
{
    int ds_num = DS.num;
    if (ds_num == f2_get_dimensionality(base_num))
    {
        ds_num--;
    }
    return DS.stack[ds_num];
}

int f4_adet_scan_feedback(string *targets, int targets_num, int &budget, ofstream &scan_log)
{
    int active_addr_num = si_adet_network_scan(targets, targets_num, budget);

    f1_print_time();
    cout << endl;
    cout << "Response number: " << active_addr_num << ", budget remains: " << budget << endl;
    f3_print_time(scan_log);
    scan_log << endl;
    scan_log << "Response number: " << active_addr_num << ", budget remains: " << budget << endl;
    
    return active_addr_num;
}

void f4_adet_replace_descendant(struct SequenceNode *&pnode, struct SequenceNode *&xi, struct SequenceNode *&xi_h)
{
    struct PreparedSpaceTreeNode *new_sptr = NULL;
    
    // 1. Check if the replacement is necessary.
    struct PreparedSpaceTreeNode *spe_ptr = pnode->node;
    struct PreparedSpaceTreeNode *spe_pptr = pnode->node->parent;
    if (f3_same_DS(spe_pptr, spe_ptr))
    {
        // Copy the TS information to the parent node.
        f3_copy_TS2parent(spe_ptr, spe_pptr);
        // Add the parent node into new_sptr.
        new_sptr = spe_pptr;
    }
    if (new_sptr == NULL)
    {
        return ;
    }

    // 2. Perform the replacement in xi and xi_h.

    // Generate an array to store information of new_node.
    int new_nodes_arr_scale = 1;
    struct PreparedSpaceTreeNode **new_nodes_arr = new struct PreparedSpaceTreeNode *[3];
    new_nodes_arr[0] = new_sptr;

    // Delete retired nodes and gather the information of SS and NDA.
    struct SequenceNode *retired_nodes = f3_get_retired_nodes(xi, xi_h, new_nodes_arr, new_nodes_arr_scale, new_sptr);
    new_sptr->SS = f3_gather_descendant_SS(retired_nodes);
    new_sptr->NDA = f3_gather_descendant_NDA(retired_nodes);
    f3_delete_retired_inseq(xi, retired_nodes);
    f3_delete_retired_inseq(xi_h, retired_nodes);
    f3_release_seq(retired_nodes);
    delete [] new_nodes_arr;

    // Update pnode.
    pnode->node = new_sptr;
}

void f4_insert(struct SequenceNode *&xi, struct SequenceNode *pnode)
{
    if (xi == NULL)
    {
        xi = pnode;
        pnode->next = NULL;
        return ;
    }

    if (f3_density_cmp(pnode, xi))
    {
        pnode->next = xi;
        xi = pnode;
        return ;
    }

    struct SequenceNode *ptr = xi->next;
    struct SequenceNode *ptr_prev = xi;
    while (ptr != NULL)
    {
        if (f3_density_cmp(pnode, ptr))
        {
            break;
        }

        ptr_prev = ptr;
        ptr = ptr->next;
    }
    ptr_prev->next = pnode;
    pnode->next = ptr;
}

void f4_set_aliased(struct PreparedSpaceTreeNode *ptr)
{
    ptr->is_aliased = true;

    for (int i = 0; i < ptr->children_num; i++)
    {
        f4_set_aliased(ptr->children[i]);
    }
}

int f4_pnode_analysis
(
    struct SequenceNode *pnode,
    struct SequenceNode *&xi,
    struct SequenceNode *&xi_h,
    int &budget,
    struct AdetParameters adet_ps,
    ofstream &addr_total_res,
    ofstream &scan_log,
    ofstream &ali_file
)
{
    // 1. Pseudorandomly select targets from its TS and perform scan_feedback(). Iterate the operation until all targets are inactive.
    while (true)
    {
        int targets_num = 0;
        struct PreparedSpaceTreeNode *spe_node = pnode->node;
        int l_dimension = f4_get_lastdimension(spe_node->DS);

        f1_print_time();
        cout << endl;
        cout << "Target region: " << spe_node->TS.expressions[0];
        int idx = 1;
        int TS_num = spe_node->TS.num;
        while (idx < TS_num)
        {
            cout << ", " << spe_node->TS.expressions[idx];
            idx++;
        }
        cout << endl;

        f3_print_time(scan_log);
        scan_log << endl;
        scan_log << "Target region: " << spe_node->TS.expressions[0];
        idx = 1;
        while (idx < TS_num)
        {
            scan_log << ", " << spe_node->TS.expressions[idx];
            idx++;
        }
        scan_log << endl;

        string *targets = f4_psurdgen_targets(spe_node->TS, l_dimension, adet_ps.ptimes, targets_num);
        budget -= targets_num;
        int active_num = f4_adet_scan_feedback(targets, targets_num, budget, scan_log);
        delete [] targets;
        if (active_num != 0)
        {
            int dimension = f3_DS_pop(spe_node);
            f3_TS_expand(spe_node, dimension);
            f4_adet_replace_descendant(pnode, xi, xi_h);
        }
        else
        {
            break;
        }
    }

    // 2. Check the scale of TS and perform different operations in different cases.
    int crip = adet_ps.crip;
    struct PreparedSpaceTreeNode *spe_node = pnode->node;
    int TS_num = spe_node->TS.num;
    string TS_expr0 = spe_node->TS.expressions[0];
    int dimensionality = f2_get_dimensionality(base_num);
    int star_num = 0;
    for (int i = 0; i < dimensionality; i++)
    {
        if (TS_expr0[i] == '*')
        {
            star_num++;
        }
    }
    double log_TS_scale = log((double )TS_num) + (double )star_num * log((double )base_num);
    double log_crip = log((double )crip);

    if (log_TS_scale > log_crip)
    {
        // Regard it as an aliased address region.

        f1_print_time();
        cout << endl;
        cout << "Aliased region: " << spe_node->TS.expressions[0];
        int idx = 1;
        int TS_num = spe_node->TS.num;
        while (idx < TS_num)
        {
            cout << ", " << spe_node->TS.expressions[idx];
            idx++;
        }
        cout << endl;

        f3_print_time(scan_log);
        scan_log << endl;
        scan_log << "Aliased region: " << spe_node->TS.expressions[0];
        idx = 1;
        while (idx < TS_num)
        {
            scan_log << ", " << spe_node->TS.expressions[idx];
            idx++;
        }
        scan_log << endl;

        f3_copy_TS2SS(spe_node);
        int dimension = f3_DS_pop(spe_node);
        f3_TS_expand(spe_node, dimension);
        f4_adet_replace_descendant(pnode, xi, xi_h);

        spe_node = pnode->node;
        for (int i = 0; i < TS_num; i++)
        {
            ali_file << spe_node->number << ", " << spe_node->TS.expressions[i] << endl;
        }

        f4_set_aliased(spe_node);
        
        if (xi == NULL)
        {
            xi = pnode;
            pnode->next = NULL;
        }
        else
        {
            struct SequenceNode *xi_ptr = xi;
            while (xi_ptr->next != NULL)
            {
                xi_ptr = xi_ptr->next;
            }
            xi_ptr->next = pnode;
            pnode->next = NULL;
        }

        int active_num = 0;
        return active_num;
    }
    else // log_TS_scale <= log_crip
    {
        // Regard it as an normal address region.

        f1_print_time();
        cout << endl;
        cout << "Normal region: " << spe_node->TS.expressions[0];
        int idx = 1;
        int TS_num = spe_node->TS.num;
        while (idx < TS_num)
        {
            cout << ", " << spe_node->TS.expressions[idx];
            idx++;
        }
        cout << endl;

        f3_print_time(scan_log);
        scan_log << endl;
        scan_log << "Normal region: " << spe_node->TS.expressions[0];
        idx = 1;
        while (idx < TS_num)
        {
            scan_log << ", " << spe_node->TS.expressions[idx];
            idx++;
        }
        scan_log << endl;

        pnode->next = NULL;
        int active_num = f4_network_scan_feedback(pnode, budget, addr_total_res, scan_log);
        f4_insert(xi, pnode);
        return active_num;
    }
}

int f4_alias_detection
(
    struct SequenceNode *&xi,
    struct SequenceNode *&xi_h, 
    int &budget, 
    struct AdetParameters adet_ps, 
    ofstream &addr_total_res,
    ofstream &scan_log, 
    ofstream &ali_file
)
{
    int active_num = 0;
    struct SequenceNode *pnode = NULL;
    pnode = f4_takeout_pnode(xi_h, adet_ps.zeta, adet_ps.pi);
    while (pnode != NULL)
    {
        f1_print_time();
        cout << "[Alias detection] Start on node: " << pnode->node->number << endl;
        f3_print_time(scan_log);
        scan_log << "[Alias detection] Start on node: " << pnode->node->number << endl;

        active_num += f4_pnode_analysis(pnode, xi, xi_h, budget, adet_ps, addr_total_res, scan_log, ali_file);

        f1_print_time();
        cout << "[Alias detection] Finished on node: " << pnode->node->number << endl;
        f3_print_time(scan_log);
        scan_log << "[Alias detection] Finished on node: " << pnode->node->number << endl;
        
        if (budget < 0)
        {
            break;
        }
        
        pnode = f4_takeout_pnode(xi_h, adet_ps.zeta, adet_ps.pi);
    }
    return active_num;
}

void f4_read_search_parameters(int &budget, int &itn_budget, struct AdetParameters &adet_ps, string treedir_name)
{
    ifstream treefile;
    treefile.open("./" + treedir_name + "/" + _SEARCH_FILE);
    
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

    // The probe number times in alias detection.
    getline(treefile, line);
    split_res = f1_str_split(line, ':');
    num_str = split_res[1];
    f3_trim(num_str);
    adet_ps.ptimes = atoi(num_str.c_str());
    split_res.clear();

    // The zeta.
    getline(treefile, line);
    split_res = f1_str_split(line, ':');
    num_str = split_res[1];
    f3_trim(num_str);
    adet_ps.zeta = atof(num_str.c_str());
    split_res.clear();

    // The pi.
    getline(treefile, line);
    split_res = f1_str_split(line, ':');
    num_str = split_res[1];
    f3_trim(num_str);
    adet_ps.pi = atof(num_str.c_str());
    split_res.clear();

    // The critical point.
    getline(treefile, line);
    split_res = f1_str_split(line, ':');
    num_str = split_res[1];
    f3_trim(num_str);
    adet_ps.crip = atoi(num_str.c_str());
    split_res.clear();
    
    treefile.close();
}

void f4_output_iris(ofstream &iris_res, int node_num, struct PreparedSpaceTreeNode *root)
{
    // Output the visualization information, based on the space tree structure.
    // If it's an aliased node, nda and density will be -1. 

    // 1. Store tree nodes into the array.
    struct PreparedSpaceTreeNode **node_arr = new struct PreparedSpaceTreeNode *[node_num + 10];
    int node_arr_scale = 0;
    node_arr[node_arr_scale++] = root;
    f3_store_node(node_arr, node_arr_scale, root);

    // 2. Output iris information to the file.
    iris_res << "base_num : " << base_num << endl;
    iris_res << "node_num : " << node_num << endl;
    iris_res << "num, inf, sup, parent_num, children_num, nda, density, subspace" << endl;
    iris_res << "1, " << root->inf << ", " << root->sup << ", 0, " << root->children_num << ", " << root->NDA;
    iris_res << ", " << f3_calc_density(root) << ", " << root->subspace << endl;
    for (int i = 1; i < node_num; i++)
    {
        int num = i + 1;
        int inf = node_arr[i]->inf;
        int sup = node_arr[i]->sup;
        int parent_num = node_arr[i]->parent->number;
        int children_num = node_arr[i]->children_num;
        string subspace = node_arr[i]->subspace;

        int nda;
        double density;
        if (node_arr[i]->is_aliased == false)
        {
            nda = node_arr[i]->NDA;
            density = f3_calc_density(node_arr[i]);
        }
        else // node_arr[i]->is_aliased == true
        {
            nda = -1;
            density = -1.0;
        }
        iris_res << num << ", " << inf << ", " << sup << ", " << parent_num << ", " << children_num << ", " << nda;
        iris_res << ", " << density << ", " << subspace << endl;
    }

    delete [] node_arr;
}

bool f4_is_aliased(string addr, struct PreparedSpaceTreeNode *root)
{
    struct PreparedSpaceTreeNode *ptr = root;
    while (true)
    {
        if (ptr->is_aliased == true)
        {
            return true;
        }
        if (ptr->children_num == 0)
        {
            return false;
        }
        
        int children_num = ptr->children_num;
        int i = 0;
        for (; i < children_num; i++)
        {
            if (f3_expression_belong(addr, ptr->children[i]->subspace))
            {
                ptr = ptr->children[i];
                break;
            }
        }
        if (i == children_num)
        {
            return false;
        }
    }
}

void f4_output_da_addrs(ifstream &total, ofstream &da, struct PreparedSpaceTreeNode *root)
{
    string line;
    while (getline(total, line))
    {
        string addr = f3_std_tran_addr(line);
        if (!f4_is_aliased(addr, root))
        {
            da << line << endl;
        }
    }
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

    // 3.1 Configure the search parameters.
    int budget, itn_budget;
    struct AdetParameters adet_ps;
    f4_read_search_parameters(budget, itn_budget, adet_ps, treedir_name);

    // 3.2 Pre-scanning.
    string res_dir_str = "./" + outres_name;
    mkdir((const char*)(res_dir_str.c_str()), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   
    // addr_res stores all found active addresses.
    ofstream addr_total_res;
    int addr_total_num = 0;
    addr_total_res.open(res_dir_str + "/" + _RES_FILE);
    ofstream scan_log;
    scan_log.open(res_dir_str + "/" + _LOG_FILE);
    addr_total_num += f4_network_scan_feedback(xi, budget, addr_total_res, scan_log);

    f1_print_time();
    cout << endl << "Pre scanning finished." << endl;
    f3_print_time(scan_log);
    scan_log << endl << "Pre scanning finished." << endl;

    ofstream ali_file;
    ali_file.open(res_dir_str + "/" + _ALI_FILE);
    ali_file << "base_num : " << base_num << endl;
    ali_file << "node_num, region" << endl;

    // 3.3 Iterative scanning, until using out the budget.
    while (budget > 0)
    {
        // Select anterior nodes from xi, based on itn_budget.
        struct SequenceNode *xi_h = f3_cut_fseg(xi, itn_budget);

        // Perform the node replacement.
        f3_replace_descendant(xi, xi_h);

        // Alias detection.
        addr_total_num += f4_alias_detection(xi, xi_h, budget, adet_ps, addr_total_res, scan_log, ali_file);
        
        if (budget > 0)
        {
            // Scan and feedback.
            addr_total_num += f4_network_scan_feedback(xi_h, budget, addr_total_res, scan_log);
        }
        
        // Merge sort.
        struct SequenceNode *tptr = f3_mergesort(xi, xi_h);
        xi = tptr;
    }

    f1_print_time();
    cout << endl << "Total scanning finished." << endl;
    cout << "Find total active addresses: " << addr_total_num << endl;
    f3_print_time(scan_log);
    scan_log << endl << "Total scanning finished." << endl;
    scan_log << "Find total active addresses: " << addr_total_num << endl;
    addr_total_res.close();
    ali_file.close();
    scan_log.close();

    // 3.4 Output dealiased active addresses.
    f1_print_time();
    cout << "[Network search] Output dealiased active addresses." << endl;
    ifstream addr_total_res_read;
    ofstream addr_da_res_write;
    addr_total_res_read.open(res_dir_str + "/" + _RES_FILE);
    addr_da_res_write.open(res_dir_str + "/" + _DARES_FILE);
    f4_output_da_addrs(addr_total_res_read, addr_da_res_write, root);
    addr_total_res_read.close();
    addr_da_res_write.close();
    f1_print_time();
    cout << "[Network search] Output dealiased active addresses finished." << endl;

    // 3.5 Output iris information.
    f1_print_time();
    cout << "[Network search] Output visualization information." << endl;
    int node_num = f3_clear_NDA(root);
    ifstream addr_da_res_read;
    addr_da_res_read.open(res_dir_str + "/" + _DARES_FILE);
    f3_count_NDA(addr_da_res_read, root);
    addr_da_res_read.close();
    ofstream iris_res;
    iris_res.open(res_dir_str + "/" + _IRIS_FILE);
    f4_output_iris(iris_res, node_num, root);
    iris_res.close();
    f1_print_time();
    cout << "[Network search] Output visualization information finished." << endl;

    // 3.6 Release space tree and sequence data.
    f1_print_time();
    cout << "[Network search] Release space tree and sequence data." << endl;
    f3_release_pspace_tree(root);
    f3_release_seq(xi);
    f1_print_time();
    cout << "[Network search] Release space tree and sequence data finished." << endl;

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
