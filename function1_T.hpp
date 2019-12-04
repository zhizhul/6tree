//
//  function1_T.hpp
//  6Tree
//
//  Created by Zhizhu Liu on 2018/12/12.
//

#ifndef function1_T_hpp
#define function1_T_hpp

#include <string>
#include <vector>

using namespace std;

void f1_print_time();

vector<string> f1_str_split(string &s, const char &c);

int f1_type_ins(string ins);

bool f1_check_type_std(string line);

bool f1_check_type_b1(string line);

bool f1_check_type_b2(string line);

bool f1_check_type_b3(string line);

bool f1_check_type_b4(string line);

bool f1_check_type_b5(string line);

bool f1_check_intype(int in_type, string line);

string f1_std_tran_b4(string line);

string f1_b1_tran_b4(string line);

string f1_b2_tran_b4(string line);

string f1_b3_tran_b1(string line);

string f1_b3_tran_b4(string line);

string f1_b5_tran_b1(string line);

string f1_b5_tran_b4(string line);

string f1_tran_in_b4(int in_type, string line);

int f1_str_cmp(string s1, string s2);

string f1_b4_tran_std(string line);

string f1_b4_tran_b1(string line);

string f1_b4_tran_b2(string line);

string f1_b1_tran_b3(string line);

string f1_b4_tran_b3(string line);

string f1_b1_tran_b5(string line);

string f1_b4_tran_b5(string line);

string f1_tran_b4_out(int out_type, string line);

void f1_work(int type1, string str2, int type3, string str4);

void f1_access(int argc, const char * argv[]);

#endif /* function1_T_hpp */
