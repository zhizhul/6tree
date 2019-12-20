6Tree
=====

Dear readers,

This is a reconstructed version which offers basic functions to discover IPv6 active addresses and aliased regions, by learning the distribution of known active addresses. Its idea is introduced in the paper ["*6Tree: Efficient dynamic discovery of active addresses in the IPv6 address space*"](https://www.sciencedirect.com/science/article/abs/pii/S1389128618312003). If you don't have access, please contact liuzhizhu17@nudt.edu.cn. Besides, you can also discuss ideas and technical details with me through the e-mail.

Its purpose is to enable asynchronous scanners to discover IPv6 active addresses more efficiently. These scanners can scan all IPv4 addresses quickly, but it's infeasible in IPv6. According to works in recent years, a solution is to design a target generation algorithm that studies known data and generates address targets that are more likely to be active. Besides, there are aliased regions on the IPv6 Internet, and they disturb the search. So an interruption mechanism is designed to measure the distribution of aliased regions and except them to improve the quality of gathered active addresses. 

By the way, the problem background can be comprehended as, in a high-dimensional vector space, valuable vectors are sparsely distributed based on unknown features, and some valuable vectors have been known. Then, how to determine a scale-limited vector set whose vectors are more probable to be valuable? For this purpose, I design the space tree search algorithm and it's suitable. I haven't found other application scenarios for this algorithm yet. Maybe it can be used elsewhere.

There are four functions in the code, and they are introduced below. Besides, the Internet-wide search capability is implemented in *scanner_interface.cpp/hpp*. In default, the code uses the system call of  [ZMapv6](https://github.com/tumi8/zmap) to implement it. So if you want to use the 4th function (-R) to perform an Internet-wide search, you need first install ZMapv6. You can also reimplement the interface by using other scanners or methods. 

Environment & Compilation
-------------------------

The code is written based on C++11 and can be compiled by g++. It can run in macOS or Linux. To compile the code, just type

> g++ -std=c++11 *.cpp -o 6tree

The 3rd function (-L) usually needs a lot of memory. It's preferable to be greater than 16GB.

Function 1: Data translation
----------------------------

> ./6tree -T -in-std/b* ***input_addrs_file*** -out-std/b* ***output_addrs_file***

Remove repeated addresses, translate them into vector representations, and sort them based on the binary order. This is a necessary pre-process for seed data since 6Tree needs the seed data being sorted, and each address vector is unique.

In the file, each line has one IPv6 address/vector. It supports five kinds of representations as below,

&nbsp; | Definition | Instance | Dimensionality
:-: | :- | :- | :-
std | colon-hexadecimal notation | 2020::2020 | /
b1 | binary mode | 001000000010...010000000100000 | 128
b2 | quaternary mode | 0200020...02000200 | 64
b3 | octal mode | 4010...020040 | 42
b4 | hexadecimal mode | 2020...02020 | 32
b5 | duotricemary mode | 040...0810 | 25

The octal mode omits the first two bits, and the duotricemary mode omits the first three bits.

For instance, if you want to translate known active addresses into seed vectors in hexadecimal mode, type

> ./6tree -T -in-std **known_active_addrs** -out-b4 **seeds_hex**

Function 2: Space tree generation
---------------------------------

> ./6tree -G -in-b* ***seeds_file*** -out-tree ***tree_folder***

The 2nd function uses seed data to generate space tree information characterizing the distribution feature of seed vectors. Besides, some parameter files are also generated for subsequent search. They are all stored in *tree_folder*, and you can adjust these parameters to run it in different scales.

Name | Definition
:- | :-
tree_info | Space tree information.
vec_seq_info | Seed vector sequence information.
search_parameters | Parameters about address search, such as budget.
scanner_parameters | Parameters about scanners, such as network protocol.

For instance, if you want to perform a space tree generation based on seed data in hexadecimal, type

> ./6tree -G -in-b4 **seeds_hex** -out-tree **tree_hex** 

Function 3: Local simulation
----------------------------

> ./6tree -L -in-tree ***tree_folder*** -test-std/b* ***test_addrs_file*** -out-res ***result_folder***

Based on the generated tree information and relative parameters, it can perform a simulation search on your local platform. *test_addrs_file* stores IPv6 addresses/vectors which are regarded as all active addresses (It also needs be processed by the 1st function (-T)). Besides, there is no alias detection in the local simulation. Files in *result_folder* include

> ./6tree -L -in-tree **tree_hex** -test-std **known_all_active_addrs** -out-res **result_hex**

Function 4: Internet-wide search
--------------------------------

与local simulation不同，结果文件夹里面的iris信息中，如果是别名结点，那么nda会记成-1。

> ./6tree -R -in-tree ***tree_folder*** -out-res ***result_folder***

> ./6tree -R -in-tree **tree_hex** -out-rs **result_hex**
