//
//  scanner_interface.hpp
//  6Tree
//
//  Created by Zhizhu Liu on 2019/12/12.
//

// Define scanner parameters and functions in the file.

#ifndef scanner_interface_hpp
#define scanner_interface_hpp

using namespace std;

// Scanner parameters which will be stored in _SCANNER_FILE.
// In this configuration, the scan command will be:
// zmap --probe-module=icmp6_echoscan --ipv6-target-file=targets.txt --output-file=result.txt --ipv6-source-ip=2001:1111::1111:2f --bandwidth=10M --cooldown-time=4
#define _SI_APP_NAME "zmap" // Application name
#define _SI_INS_NUM 6 // The number of secondary instructions
#define _SI_INS_PM "--probe-module=icmp6_echoscan"
#define _SI_INS_STEP_TF "--ipv6-target-file=targets.txt" // The file name should be same as _SI_STEP_TF_FILE
#define _SI_INS_STEP_RES "--output-file=result.txt" // The file name should be same as _SI_STEP_RES_FILE
#define _SI_INS_SIP "--ipv6-source-ip=2001:1111::1111:2f"
#define _SI_INS_BW "--bandwidth=10M"
#define _SI_INS_CT "--cooldown-time=4"

// File name of address targets in one step
#define _SI_STEP_TF_FILE "targets.txt"
// File name of discovered active addresses in one step // -- need work: 在真实扫描条件下，一步获取到的活跃地址要进行排序去重，因为有可能目标IP地址返回了多个ping响应报文。
#define _SI_STEP_RES_FILE "result.txt"

void si_output_scanner_parameters(string outdir_name);

#endif /* scanner_interface_hpp */
