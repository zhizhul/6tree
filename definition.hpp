//
//  definition.hpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/2/22.

// Define parameters, data structures, global variables ... in the file.

#ifndef definition_hpp
#define definition_hpp

#include <string>

using namespace std;

// Secondary instructions
#define _INS_INSTD 1 // -in-std
#define _INS_INB1 2 // -in-b1
#define _INS_INB2 3 // -in-b2
#define _INS_INB3 4 // -in-b3
#define _INS_INB4 5 // -in-b4
#define _INS_INB5 6 // -in-b5
#define _INS_OUTSTD 7 // -out-std
#define _INS_OUTB1 8 // -out-b1
#define _INS_OUTB2 9 // -out-b2
#define _INS_OUTB3 10 // -out-b3
#define _INS_OUTB4 11 // -out-b4
#define _INS_OUTB5 12 // -out-b5
#define _INS_OUTTREE 13 // -out-tree
#define _INS_INTREE 14 // -in-tree
#define _INS_TESTSTD 15 // -test-std
#define _INS_TESTB1 16 // -test-b1
#define _INS_TESTB2 17 // -test-b2
#define _INS_TESTB3 18 // -test-b3
#define _INS_TESTB4 19 // -test-b4
#define _INS_TESTB5 20 // -test-b5
#define _INS_OUTRES 21 // -out-res
#define _INS_CONFIG 22 // -config

// Space tree node
struct SpaceTreeNode
{
    // Node number
    int number;
    // Lower bound index
    int inf;
    // Upper bound index
    int sup;
    // Parent node pointer
    struct SpaceTreeNode *parent;
    // Child node pointers
    struct SpaceTreeNode **children;
    // Number of child nodes
    int children_num;
};

// File name of generated space tree information
#define _TREE_FILE "tree_info"
// File name of vector sequence
#define _ARR_FILE "vec_seq_info"
// File name of scan parameters
#define _PAR_FILE "scan_parameters"

// Pre-defined budget
#define _SCAN_BUDGET 10000000
// Pre-defined budget in each iteration
#define _SCAN_STEP_BUDGET 2000000

// File name of discovered active addresses
#define _RES_FILE "discovered_addrs"
// File name of discovered active addresses in one step
#define _STEP_RES_FILE "active_addrs"
// File name of detected alias regions
#define _ALI_FILE "alias_regions"
// File name of Iris information
#define _IRIS_FILE "iris_info"
// File name of scan log
#define _LOG_FILE "scan_log"

// Dimension stack
struct DimenStack
{
    // Scale
    int num;
    // Stack
    // The index starts from 0.
    int *stack;
};

// Vector region
struct VectorRegion
{
    // Scale, for instance, 3. Usually it's 1.
    int num;
    // Region expression array, for instance, {56**...8, 78**...3, ef**...2}.
    // The index starts from 0.
    string *expressions;
};

// Space tree node for dynamic search
struct PreparedSpaceTreeNode
{
    // Node number
    int number;
    // Lower bound index
    int inf;
    // Upper bound index
    int sup;
    // Region expression, for instance, 20030**40...00*...*.
    string subspace;
    // Parent node pointer
    struct PreparedSpaceTreeNode *parent;
    // Child node pointers
    struct PreparedSpaceTreeNode **children;
    // Number of child nodes
    int children_num;
    
    // Dimensional stack
    struct DimenStack DS;
    // Target address set
    struct VectorRegion TS;
    // Scanned address set
    struct VectorRegion SS;
    // Number of discovered addresses
    int NDA;
};

// Space tree node sequence node
struct SequenceNode
{
    // Space tree node pointer
    struct PreparedSpaceTreeNode *node;
    // Subsequent node pointer
    struct SequenceNode *next;
};

// Search tree node for local test, storing information about known all addresses
struct SearchTreeNode
{
    // Number of active addresses in the region of the node, -- need work: 代码写完后检查一下，这里面有些数据可能用不到（比如说level），就可以删去了
    int NAA;
    // Level of the node, the root node level is 0.
    int level;
    // Corresponding character of the node, the root node doesn't have a character.
    char ch;
    // If it's a leaf node
    bool is_leaf;
    // Region expression, for instance, 20030**40...00*...*.
    string expression;
    // Parent node pointer
    struct SearchTreeNode *parent;
    // Child node pointers
    struct SearchTreeNode **children;
    // Number of child nodes, when is_leaf is true, it's 0, nor it's equal to base_num.
    int children_num;
};

// Region tree node for storing information about target addresses
struct RegionTreeNode
{
    // Region expression, for instance, 20030**40...00*...* in hexadecimal.
    string expression;
    // If it's a leaf node
    bool is_leaf;
    // Space tree node pointer, the region of the region tree node belongs to the region of the space tree node.
    struct PreparedSpaceTreeNode *spe_node_ptr;
    // Parent node pointer
    struct RegionTreeNode *parent;
    // Child node pointers
    struct RegionTreeNode **children;
    // Number of child nodes, when is_leaf is true, it's 0, nor it's equal to base_num.
    int children_num;
};

// Base number, for instance, 16 in hexadecimal.
extern int base_num;

#endif  /* definition_hpp */
