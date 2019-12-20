6Tree
=====

This is a reconstructed version which has basic functions to discover IPv6 active addresses and aliased regions, by learning the distribution of known active addresses. Its idea is introduced in the paper ["*6Tree: Efficient dynamic discovery of active addresses in the IPv6 address space*"](https://www.sciencedirect.com/science/article/abs/pii/S1389128618312003). If you don't have access, please contact liuzhizhu17@nudt.edu.cn, and you can also discuss ideas and technical details with me through the e-mail.

Its purpose is to enable asynchronous scanners to discover IPv6 active addresses more efficiently. These scanners can scan all IPv4 addresses quickly, but it's infeasible in IPv6. According to works in recent years, a solution is to design a target generation algorithm that studies known data and generates address targets that are more likely to be active. Besides, there are aliased regions on the IPv6 Internet, and they disturb the search. So an interruption mechanism is designed to measure the distribution of aliased regions and except them to improve the quality of gathered active addresses. 

By the way, the problem background can be comprehended as, in a high-dimensional vector space, valuable vectors are sparsely distributed based on unknown features, and some valuable vectors have been known. Then, how to determine a scale-limited vector set whose vectors are more probable to be valuable? For this purpose, I design the space tree search algorithm and it's suitable. I haven't found other application scenarios for this algorithm yet. Maybe it can be used elsewhere.

There are four functions in the code, and they are introduced below. Besides, the Internet-wide search capability is implemented in *scanner_interface.cpp/hpp*. In default, the code uses the system call of  [ZMapv6](https://github.com/tumi8/zmap) to implement it. So if you want to use the 4th function (-R) to perform an Internet-wide search, you need first install ZMapv6. You can also reimplement the interface by using other scanners or methods. 

Environment & Compile
---------------------

The code is written based on C++11 and can be compiled by g++. It can run in macOS or Linux. To compile the code, just type

> g++ -std=c++11 *.cpp -o 6tree

The 3rd function (-L) usually needs a lot of memory. It's preferable to be greater than 16GB.

Function 1: Data translation
----------------------------

> ./6tree -T -in-std/b* ***input_addrs*** -out-std/b* ***output_addrs***

基本功能是把带有IPv6地址的文件进行排序（基于二进制顺序）去重后输出，因为6Tree算法必须要求输入的种子IPv6地址是经过排序和去重的。文件中要求一行放置一个IPv6地址。这里代码支持5种进制模式，其中8进制会省略掉前面2位（the first two bits），32进制会省略掉前面3位，例子如下（这里或许可以做一张表）：

 std, colon-hexadecimal, ...

 b1, binary, ...

 b2, quaternary, ...

 b3, octal, ...

 b4, hexadecimal, ...

 b5, duotricemary, ...

> ./6tree -T -in-std **known_active_addrs** -out-b4 **seeds_hex**

Function 2: Space tree generation
---------------------------------

结果以文件夹表示，其中包含了生成的空间树基本信息。此外代码默认还把扫描器和扫描的相关参数文件放在了这个文件夹里面。基于这个文件夹中的数据就可以用来支持活跃地址与别名区测量了。

代码默认在*rseult_file*文件夹里面生成这么几个文件：...（还是做个表格吧）

> ./6tree -G -in-b* *seeds_file* -out-tree *tree_folder*

> ./6tree -G -in-b4 seeds_hex -out-tree tree_hex
                

Function 3: Local simulation
----------------------------

1. *test_addrs*中的地址将会作为假设的全部活跃地址
2. 结果文件夹里面有这么几个文件...

> ./6tree -L -in-tree *tree_folder* -test-std/b* *test_addrs* -out-res *result_folder*

> ./6tree -L -in-tree tree_hex -test-std known_all_active_addrs -out-res result_hex

Function 4: Internet-wide search
--------------------------------

与local simulation不同，结果文件夹里面的iris信息中，如果是别名结点，那么nda会记成-1。

> ./6tree -R -in-tree *tree_folder* -out-res *result_folder*

> ./6tree -R -in-tree tree_hex -out-rs result_hex