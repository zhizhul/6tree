//
//  function1_T.cpp
//  6Tree
//
//  Created by Zhizhu Liu on 2018/12/12.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include "definition.hpp"
#include "function1_T.hpp"

using namespace std;

void f1_print_time()
{
    // Output the time.
    time_t now = time(0);
    tm *ltm = localtime(&now);
    cout << "[" << (1900 + ltm->tm_year) << "/" << (1 + ltm->tm_mon) << "/" << ltm->tm_mday << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "]";
}

vector<string> f1_str_split(string &s, const char &c)
{
    // Split the string.
    string buff{ "" };
    vector<string> v;
    
    for (auto n : s)
    {
        if (n != c)
        {
            buff += n;
        }
        else if (n == c)
        {
            v.push_back(buff);
            buff = "";
        }
    }
    if (buff != "")
    {
        v.push_back(buff);
    }
    return v;
}

int f1_type_ins(string ins)
{
    // Analyze the type of the instruction.
    if (ins == "-in-std")
    {
        return _INS_INSTD;
    }
    if (ins == "-in-b1")
    {
        return _INS_INB1;
    }
    if (ins == "-in-b2")
    {
        return _INS_INB2;
    }
    if (ins == "-in-b3")
    {
        return _INS_INB3;
    }
    if (ins == "-in-b4")
    {
        return _INS_INB4;
    }
    if (ins == "-in-b5")
    {
        return _INS_INB5;
    }
    if (ins == "-out-std")
    {
        return _INS_OUTSTD;
    }
    if (ins == "-out-b1")
    {
        return _INS_OUTB1;
    }
    if (ins == "-out-b2")
    {
        return _INS_OUTB2;
    }
    if (ins == "-out-b3")
    {
        return _INS_OUTB3;
    }
    if (ins == "-out-b4")
    {
        return _INS_OUTB4;
    }
    if (ins == "-out-b5")
    {
        return _INS_OUTB5;
    }
    if (ins == "-out-tree")
    {
        return _INS_OUTTREE;
    }
    if (ins == "-in-tree")
    {
        return _INS_INTREE;
    }
    if (ins == "-test-std")
    {
        return _INS_TESTSTD;
    }
    if (ins == "-test-b1")
    {
        return _INS_TESTB1;
    }
    if (ins == "-test-b2")
    {
        return _INS_TESTB2;
    }
    if (ins == "-test-b3")
    {
        return _INS_TESTB3;
    }
    if (ins == "-test-b4")
    {
        return _INS_TESTB4;
    }
    if (ins == "-test-b5")
    {
        return _INS_TESTB5;
    }
    if (ins == "-out-res")
    {
        return _INS_OUTRES;
    }
    if (ins == "-ptl")
    {
        return _INS_PTL;
    }
    return 0;
}

bool f1_check_type_std(string line)
{
    // Check if the string conform the colon-hexadecimal notation.
    int len = (int )line.length();
    int single_time = 0;
    int double_time = 0;
    for (int i = 0; i < len; i++)
    {
        if (line[i] == ':')
        {
            single_time++;
            if (i + 1 < len && line[i + 1] == ':')
            {
                double_time++;
            }
        }
    }
    
    if (double_time > 1)
    {
        return false;
    }
    if (double_time == 1 && single_time > 7)
    {
        return false;
    }
    if (double_time == 0 && single_time != 7)
    {
        return false;
    }
    
    if (double_time == 0)
    {
        vector<string> split_res = f1_str_split(line, ':');
        int split_num = (int )split_res.size();
        if (split_num != 8)
        {
            return false;
        }
        for (int i = 0; i < 8; i++)
        {
            string str = split_res[i];
            int str_len = (int )str.length();
            if (str_len > 4)
            {
                split_res.clear();
                return false;
            }
            for (int j = 0; j < str_len; j++)
            {
                if (str[j] >= '0' && str[j] <= '9')
                {
                    ;
                }
                else if (str[j] >= 'A' && str[j] <= 'F')
                {
                    ;
                }
                else if (str[j] >= 'a' && str[j] <= 'f')
                {
                    ;
                }
                else
                {
                    split_res.clear();
                    return false;
                }
            }
        }
        split_res.clear();
    }
    else // double_time == 1
    {
        vector<string> split_res = f1_str_split(line, ':');
        int split_num = (int )split_res.size();
        if (split_num > 8)
        {
            split_res.clear();
            return false;
        }
        for (int i = 0; i < split_num; i++)
        {
            string str = split_res[i];
            int str_len = (int )str.length();
            if (str_len > 4)
            {
                split_res.clear();
                return false;
            }
            if (str_len == 0)
            {
                continue;
            }
            for (int j = 0; j < str_len; j++)
            {
                if (str[j] >= '0' && str[j] <= '9')
                {
                    ;
                }
                else if (str[j] >= 'A' && str[j] <= 'F')
                {
                    ;
                }
                else if (str[j] >= 'a' && str[j] <= 'f')
                {
                    ;
                }
                else
                {
                    split_res.clear();
                    return false;
                }
            }
        }
        split_res.clear();
    }
    return true;
}

bool f1_check_type_b1(string line)
{
    // Check if the string conform the base-2 mode.
    if (line.length() != 128)
    {
        return false;
    }
    for (int i = 0; i < 128; i++)
    {
        if (!(line[i] >= '0' && line[i] <= '1'))
        {
            return false;
        }
    }
    return true;
}

bool f1_check_type_b2(string line)
{
    // Check if the string conform the base-4 mode.
    if (line.length() != 64)
    {
        return false;
    }
    for (int i = 0; i < 64; i++)
    {
        if (!(line[i] >= '0' && line[i] <= '3'))
        {
            return false;
        }
    }
    return true;
}

bool f1_check_type_b3(string line)
{
    // Check if the string conform the base-8 mode. The first two bits (00) will be omitted.
    if (line.length() != 42)
    {
        return false;
    }
    for (int i = 0; i < 42; i++)
    {
        if (!(line[i] >= '0' && line[i] <= '7'))
        {
            return false;
        }
    }
    return true;
}

bool f1_check_type_b4(string line)
{
    // Check if the string conform the base-16 mode.
    if (line.length() != 32)
    {
        return false;
    }
    for (int i = 0; i < 32; i++)
    {
        if (!((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')))
        {
            return false;
        }
    }
    return true;
}

bool f1_check_type_b5(string line)
{
    // Check if the string conform the base-32 mode. The first three bits (000) will be omitted.
    if (line.length() != 25)
    {
        return false;
    }
    for (int i = 0; i < 25; i++)
    {
        if (!((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'V') || (line[i] >= 'a' && line[i] <= 'v')))
        {
            return false;
        }
    }
    return true;
}

bool f1_check_intype(int in_type, string line)
{
    // Check if the string conform the mode.
    if (in_type == _INS_INSTD)
    {
        return f1_check_type_std(line);
    }
    if (in_type == _INS_INB1)
    {
        return f1_check_type_b1(line);
    }
    if (in_type == _INS_INB2)
    {
        return f1_check_type_b2(line);
    }
    if (in_type == _INS_INB3)
    {
        return f1_check_type_b3(line);
    }
    if (in_type == _INS_INB4)
    {
        return f1_check_type_b4(line);
    }
    if (in_type == _INS_INB5)
    {
        return f1_check_type_b5(line);
    }
    
    return false;
}

string f1_std_tran_b4(string line)
{
    // Translate the colon-hexadecimal notation into the base-16 mode notation.
    int len = (int )line.length();
    int single_time = 0;
    int double_time = 0;
    for (int i = 0; i < len; i++)
    {
        if (line[i] == ':')
        {
            single_time++;
            if (i + 1 < len && line[i + 1] == ':')
            {
                double_time++;
            }
        }
    }
    
    if (double_time == 0)
    {
        string res_str = "";
        vector<string> split_res = f1_str_split(line, ':');
        for (int i = 0; i < 8; i++)
        {
            string str = split_res[i];
            int str_len = (int )str.length();
            for (int j = 0; j < 4 - str_len; j++)
            {
                res_str += '0';
            }
            res_str += str;
        }
        split_res.clear();
        for (int i = 0; i < 32; i++)
        {
            if (res_str[i] >= 'A' && res_str[i] <= 'F')
            {
                res_str[i] += ('a' - 'A');
            }
        }
        return res_str;
    }
    else // double_time == 1
    {
        vector<string> split_res = f1_str_split(line, ':');
        int split_num = (int )split_res.size();
        
        string *str_arr = new string [8];
        int str_arr_scale = 0;
        int null_time = 0;
        for (int i = 0; i < split_num; i++)
        {
            if (split_res[i] == "")
            {
                if (null_time == 0)
                {
                    null_time++;
                    str_arr[str_arr_scale++] = "";
                }
            }
            else
            {
                str_arr[str_arr_scale++] = split_res[i];
            }
        }
        split_res.clear();
        
        int add_num = 9 - str_arr_scale;
        string res_str = "";
        for (int i = 0; i < str_arr_scale; i++)
        {
            if (str_arr[i] == "")
            {
                for (int j = 0; j < add_num; j++)
                {
                    res_str += "0000";
                }
            }
            else
            {
                string str = str_arr[i];
                int str_len = (int )str.length();
                for (int j = 0; j < 4 - str_len; j++)
                {
                    res_str += '0';
                }
                res_str += str;
            }
        }
        delete [] str_arr;
        for (int i = 0; i < 32; i++)
        {
            if (res_str[i] >= 'A' && res_str[i] <= 'F')
            {
                res_str[i] += ('a' - 'A');
            }
        }
        return res_str;
    }
}

string f1_b1_tran_b4(string line)
{
    // Translate the base-2 mode notation into the base-16 mode notation.
    string res_str = "";
    for (int i = 0; i < 32; i++)
    {
        int num = 0;
        num += ((line[i * 4 + 0] - '0') << 3);
        num += ((line[i * 4 + 1] - '0') << 2);
        num += ((line[i * 4 + 2] - '0') << 1);
        num += ((line[i * 4 + 3] - '0') << 0);
        if (num >= 10)
        {
            res_str += (num - 10 + 'a');
        }
        else
        {
            res_str += (num + '0');
        }
    }
    return res_str;
}

string f1_b2_tran_b4(string line)
{
    // Translate the base-4 mode notation into the base-16 mode notation.
    string res_str = "";
    for (int i = 0; i < 32; i++)
    {
        int num = 0;
        num += ((line[i * 2 + 0] - '0') << 2);
        num += ((line[i * 2 + 1] - '0') << 0);
        if (num >= 10)
        {
            res_str += (num - 10 + 'a');
        }
        else
        {
            res_str += (num + '0');
        }
    }
    return res_str;
}

string f1_b3_tran_b1(string line)
{
    // Translate the base-8 mode notation into the base-2 mode notation.
    string bin_str = "00";
    string arr[] = { "000", "001", "010", "011", "100", "101", "110", "111" };
    for (int i = 0; i < 42; i++)
    {
        int num = line[i] - '0';
        bin_str += arr[num];
    }
    return bin_str;
}

string f1_b3_tran_b4(string line)
{
    // Translate the base-8 mode notation into the base-16 mode notation.
    return f1_b1_tran_b4(f1_b3_tran_b1(line));
}

string f1_b5_tran_b1(string line)
{
    // Translate the base-32 mode notation into the base-2 mode notation.
    string bin_str = "001";
    string arr[] = { "00000", "00001", "00010", "00011", "00100", "00101", "00110", "00111",
        "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111",
        "10000", "10001", "10010", "10011", "10100", "10101", "10110", "10111",
        "11000", "11001", "11010", "11011", "11100", "11101", "11110", "11111" };
    for (int i = 0; i < 25; i++)
    {
        if (line[i] >= '0' && line[i] <= '9')
        {
            int num = line[i] - '0';
            bin_str += arr[num];
        }
        else if (line[i] >= 'A' && line[i] <= 'V')
        {
            int num = line[i] - 'A' + 10;
            bin_str += arr[num];
        }
        else // line[i] >= 'a' && line[i] <= 'v'
        {
            int num = line[i] - 'a' + 10;
            bin_str += arr[num];
        }
    }
    return bin_str;
}

string f1_b5_tran_b4(string line)
{
    // Translate the base-32 mode notation into the base-16 mode notation.
    return f1_b1_tran_b4(f1_b5_tran_b1(line));
}

string f1_tran_in_b4(int in_type, string line)
{
    // Translate the string into the base-16 mode notation.
    if (in_type == _INS_INSTD)
    {
        return f1_std_tran_b4(line);
    }
    else if (in_type == _INS_INB1)
    {
        return f1_b1_tran_b4(line);
    }
    else if (in_type == _INS_INB2)
    {
        return f1_b2_tran_b4(line);
    }
    else if (in_type == _INS_INB3)
    {
        return f1_b3_tran_b4(line);
    }
    else if (in_type == _INS_INB4)
    {
        return line;
    }
    else // in_type == _INS_INB5
    {
        return f1_b5_tran_b4(line);
    }
}

int f1_str_cmp(string s1, string s2)
{
    return s1.compare(s2) < 0;
}

string f1_b4_tran_std(string line)
{
    // Translate the base-16 mode notation into the colon-hexadecimal notation.
    string res_str = "";
    for (int i = 0; i < 7; i++)
    {
        res_str += line[i * 4 + 0];
        res_str += line[i * 4 + 1];
        res_str += line[i * 4 + 2];
        res_str += line[i * 4 + 3];
        res_str += ':';
    }
    res_str += line[28];
    res_str += line[29];
    res_str += line[30];
    res_str += line[31];
    return res_str;
}

string f1_b4_tran_b1(string line)
{
    // Translate the base-16 mode notation into the base-2 mode notation.
    string res_str = "";
    string arr[] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
        "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
    for (int i = 0; i < 32; i++)
    {
        if (line[i] >= '0' && line[i] <= '9')
        {
            int num = line[i] - '0';
            res_str += arr[num];
        }
        else if (line[i] >= 'A' && line[i] <= 'F')
        {
            int num = line[i] - 'A' + 10;
            res_str += arr[num];
        }
        else // line[i] >= 'a' && line[i] <= 'f'
        {
            int num = line[i] - 'a' + 10;
            res_str += arr[num];
        }
    }
    return res_str;
}

string f1_b4_tran_b2(string line)
{
    // Translate the base-16 mode notation into the base-4 mode notation.
    string res_str = "";
    string arr[] = { "00", "01", "02", "03", "10", "11", "12", "13",
        "20", "21", "22", "23", "30", "31", "32", "33" };
    for (int i = 0; i < 32; i++)
    {
        if (line[i] >= '0' && line[i] <= '9')
        {
            int num = line[i] - '0';
            res_str += arr[num];
        }
        else if (line[i] >= 'A' && line[i] <= 'F')
        {
            int num = line[i] - 'A' + 10;
            res_str += arr[num];
        }
        else // line[i] >= 'a' && line[i] <= 'f'
        {
            int num = line[i] - 'a' + 10;
            res_str += arr[num];
        }
    }
    return res_str;
}

string f1_b1_tran_b3(string line)
{
    // Translate the base-2 mode notation into the base-8 mode notation.
    string res_str = "";
    for (int i = 0; i < 42; i++)
    {
        int num = 0;
        num += ((line[i * 3 + 2] - '0') << 2);
        num += ((line[i * 3 + 3] - '0') << 1);
        num += ((line[i * 3 + 4] - '0') << 0);
        res_str += ('0' + num);
    }
    return res_str;
}

string f1_b4_tran_b3(string line)
{
    // Translate the base-16 mode notation into the base-8 mode notation.
    return f1_b1_tran_b3(f1_b4_tran_b1(line));
}

string f1_b1_tran_b5(string line)
{
    // Translate the base-2 mode notation into the base-32 mode notation.
    string res_str = "";
    for (int i = 0; i < 25; i++)
    {
        int num = 0;
        num += ((line[i * 5 + 3] - '0') << 4);
        num += ((line[i * 5 + 4] - '0') << 3);
        num += ((line[i * 5 + 5] - '0') << 2);
        num += ((line[i * 5 + 6] - '0') << 1);
        num += ((line[i * 5 + 7] - '0') << 0);
        if (num <= 9)
        {
            res_str += ('0' + num);
        }
        else // num >= 10
        {
            res_str += ('a' + num - 10);
        }
    }
    return res_str;
}

string f1_b4_tran_b5(string line)
{
    // Translate the base-16 mode notation into the base-32 mode notation.
    return f1_b1_tran_b5(f1_b4_tran_b1(line));
}

string f1_tran_b4_out(int out_type, string line)
{
    // Translate the string.
    if (out_type == _INS_OUTSTD)
    {
        return f1_b4_tran_std(line);
    }
    else if (out_type == _INS_OUTB1)
    {
        return f1_b4_tran_b1(line);
    }
    else if (out_type == _INS_OUTB2)
    {
        return f1_b4_tran_b2(line);
    }
    else if (out_type == _INS_OUTB3)
    {
        return f1_b4_tran_b3(line);
    }
    else if (out_type == _INS_OUTB4)
    {
        return line;
    }
    else // out_type == _INS_OUTB5
    {
        return f1_b4_tran_b5(line);
    }
}

void f1_work(int type1, string str2, int type3, string str4)
{
    // Analyze the instruction.
    string infile_name;
    string outfile_name;
    int in_type;
    int out_type;
    if (type1 >= _INS_INSTD && type1 <= _INS_INB5)
    {
        // str2 is the input file name, str4 is the output file name.
        infile_name = str2;
        outfile_name = str4;
        in_type = type1;
        out_type = type3;
    }
    else
    {
        // str2 is the output file name, str4 is the input file name.
        infile_name = str4;
        outfile_name = str2;
        in_type = type3;
        out_type = type1;
    }
    
    f1_print_time();
    cout << "[Translation] Start translation." << endl;
    f1_print_time();
    cout << "[Translation] Input and translate data." << endl;
    
    // Count the number of lines, and build a corresponding array.
    ifstream infile;
    string line;
    int arr_scale = 0;
    infile.open(infile_name);
    while (getline(infile, line))
    {
        arr_scale++;
    }
    infile.close();
    string *arr = new string [arr_scale + 10];
    
    // Translate each normal string into quaternary mode, then put it into the array.
    arr_scale = 0;
    infile.open(infile_name);
    while (getline(infile, line))
    {
        if (f1_check_intype(in_type, line))
        {
            arr[arr_scale++] = f1_tran_in_b4(in_type, line);
        }
    }
    infile.close();
    
    f1_print_time();
    cout << "[Translation] Input and translate data finished." << endl;
    f1_print_time();
    cout << "[Translation] Sort and unique." << endl;
    
    // Sort and unique, then translate strings and output the result.
    sort(arr, arr + arr_scale, f1_str_cmp);
    int new_arr_scale;
    new_arr_scale = (int)(unique(arr, arr + arr_scale) - arr);
    arr_scale = new_arr_scale;
    
    f1_print_time();
    cout << "[Translation] Sort and unique finished." << endl;
    f1_print_time();
    cout << "[Translation] Translate and output data." << endl;
    
    ofstream outfile;
    outfile.open(outfile_name);
    for (int i = 0; i < arr_scale; i++)
    {
        outfile << f1_tran_b4_out(out_type, arr[i]) << endl;
    }
    outfile.close();
    delete [] arr;
    
    f1_print_time();
    cout << "[Translation] Translate and output data finished." << endl;
    f1_print_time();
    cout << "[Translation] Translation finished." << endl;
}

void f1_access(int argc, const char * argv[])
{
    // 6tree -T (-in-std/b* *input file name*) (-out-std/b* *output file name*)
    
    // 1. In the file, each line has only one string which means the address vector. String which does not conform to the mode will be omitted.
    // 2. Sort and unique also be performed in the translation.
    // 3. The base modes include binary (b2), quatenary (b2), ..., base-32 (b5). b3 omits the first two bits (00), b5 omits the first three bits (001).
    // 4. The standard mode means colon-hexadecimal notation (std).
    
    if (argc != 6)
    {
        cout << "[Error] Function instruction is incorrect." << endl;
        return;
    }
    
    // Check instruction correctness.
    int type1 = f1_type_ins(string(argv[2]));
    int type3 = f1_type_ins(string(argv[4]));
    int pool_ins_in = 0;
    int pool_ins_out = 0;
    if (type1 >= _INS_INSTD && type1 <= _INS_INB5)
    {
        pool_ins_in++;
    }
    if (type3 >= _INS_INSTD && type3 <= _INS_INB5)
    {
        pool_ins_in++;
    }
    if (type1 >= _INS_OUTSTD && type1 <= _INS_OUTB5)
    {
        pool_ins_out++;
    }
    if (type3 >= _INS_OUTSTD && type3 <= _INS_OUTB5)
    {
        pool_ins_out++;
    }
    if (pool_ins_in != 1 || pool_ins_out != 1)
    {
        cout << "[Error] Function instruction is incorrect." << endl;
        return;
    }
    
    // Start the translation.
    f1_work(type1, string(argv[3]), type3, string(argv[5]));
}
