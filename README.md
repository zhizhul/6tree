6Tree
=====

Now I am working on README.md (manuscript below), coming soon ...

这是6Tree的重构代码（Reconstructed version）能够实现活跃地址与别名区测量能力（言简意赅地说清楚功能，一句话做到，然后第二段再详细讲），并以便于读者和从业者了解与使用。想了解这种方法的idea，请阅读论文...中（如果没有访问渠道的话请给作者发邮件：...。）=>(Dont't have an access to the paper? Please send an e-mail to me: liuzhizhu17@nudt.edu.cn)。您也可以通过邮件(through the mail)与我交流思路和技术细节(technical details)。

这种方法的目的是为了在IPv6环境下适用ZMap等异步扫描器寻找活跃地址。高速扫描器可以遍历IPv4来找地址，但是IPv6不行。所以根据近年来相关工作的思路，一种办法是设计算法学习已知活跃地址的分布特征来训练高速扫描器有方向地找到活跃地址。根据前人的测量情况，IPv6中还存在大规模的别名区，会影响活跃地址发现，因此我在设计6Tree时还考虑了把别名区测量出来并排除出去，从而提高搜集的活跃地址数据质量。

这实际上是一种在高维空间下面寻找有价值向量数据的算法，对于IPv6非常适用，不过作者还没找到其他不错的应用环境。或许可以用到AI里面。

代码中包含四种功能，涵盖了6Tree的基本能力，将在下面阐述。其中实现了扫描器接口*scanner_interface.cpp/hpp*，用户可以自己在里面定义实现自己的高速扫描工具。默认情况下，代码采用ZMapv6（给出github的超链接）并通过命令行调用的方式来实现高速扫描能力，因此如果想使用第四个功能，也就是真实网络扫描，就需要先安装ZMapv6。

Environment & Compile
---------------------

代码是基于C++11标准写的，可以使用GNU编译器编译，能够运行在MacOS或者Linux平台下面。将代码下载下来之后，使用以下指令即可编译：

> g++ -std=c++11 *.cpp -o 6tree

Function 1: Data translation
----------------------------

基本功能是把带有IPv6地址的文件进行排序（基于二进制顺序）去重后输出，因为6Tree算法必须要求输入的种子IPv6地址是经过排序和去重的。文件中要求一行放置一个IPv6地址。这里代码支持5种进制模式，其中8进制会省略掉前面2位（the first two bits），32进制会省略掉前面3位，例子如下（这里或许可以做一张表）：

 std, colon-hexadecimal, ...

 b1, binary, ...

 b2, quaternary, ...

 b3, octal, ...

 b4, hexadecimal, ...

 b5, duotricemary, ...

> ./6tree -T -in-std/b* *input_addrs* -out-std/b* *output_addrs*

> ./6tree -T -in-std known_active_addrs -out-b4 seeds_hex

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