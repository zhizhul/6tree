//
//  scanner_interface.cpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/12/12.
//

// Define scanner interface functions in the file.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdio>
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

void si_read_scanner_parameters(string treedir_name)
{
    ifstream pfile;
    pfile.open("./" + treedir_name + "/" + _SCANNER_FILE);
    // -- need work: 写到了这里。
    pfile.close();
}

void si_output_scanner_parameters(string outdir_name)
{
    // Be used in function2_G.cpp

    // Output pre-defined scanner parameters into the configuration file.
    // Users can adjust parameters in the file, but incorrect adjustments might trigger errors.
    
    ofstream outfile;
    outfile.open("./" + outdir_name + "/" + _SCANNER_FILE);

    outfile << "app_name : " << _SI_APP_NAME << endl;
    outfile << "ins_num : " << _SI_INS_NUM << endl;
    outfile << _SI_INS_PM << endl;
    outfile << _SI_INS_STEP_TF << endl;
    outfile << _SI_INS_STEP_RES << endl;
    outfile << _SI_INS_SIP << endl;
    outfile << _SI_INS_BW << endl;
    outfile << _SI_INS_CT << endl;

    outfile.close();
}