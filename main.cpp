//
//  main.cpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/11/16.
//

// Programming language standard: C++11.
// Operation system environment: MacOS, Linux.

#include <iostream>
#include <ctime>
#include "definition.hpp"
#include "function1_T.hpp"
#include "function2_G.hpp"
#include "function3_L.hpp"

using namespace std;

int work(int argc, const char * argv[])
{
    // Analyze the first instruction, if it's not T/G/L/R, exit.
    if (argc <= 1 || string(argv[1]).length() != 2 || argv[1][0] != '-')
    {
        cout << "[Error] The first instruction is incorrect." << endl;
        return 0;
    }
    else
    {
        switch (argv[1][1]) {
            case 'T':
                // File translation:
                // 6tree -T (-in-std/b* *input file name*) (-out-std/b* *output file name*)
                f1_access(argc, argv);
                break;
            case 'G':
                // Space tree generation:
                // 6tree -G (-in-b* *address file name*) (-out-tree *output folder name*)
                f2_access(argc, argv);
                break;
            case 'L':
                // Local simulation search based on space tree:
                // 6tree -L (-in-tree *tree folder name*) (-test-std/b* *test address file name*) (-out-res *result folder name*)
                f3_access(argc, argv);
                break;
            case 'R':
                // 基于空间树展开真实扫描（会检查是否ZMapv6可运行，会检查放置结果的文件夹是否不存在或者存在且非空=>改为仅配置接口）：
                // 6tree -R -in-tree *输入文件夹名* -ptl *采用的协议参数，要与ZMapv6设置一致* -out-res *放置扫描结果的文件夹名*
                
                // -- need work
                break;
            default:
                // Analyze the first instruction, if it's not T/G/L/R, exit.
                cout << "[Error] The first instruction is incorrect." << endl;
                return 0;
        }
    }
    return 0;
}

int main(int argc, const char * argv[])
{
    work(argc, argv);
    return 0;
}
