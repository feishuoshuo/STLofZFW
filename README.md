
## Background
- **项目描述**：
本项目使用 C++11，在阅读侯捷的《STL 源码解析》的基础上，依照部分 SGI 的源码实现，自主研发了一个轻量级的小型 STL 库。
- **主要工作**：制作简易版 allocator 空间配置器、标准库中相关容器、配套算法、适配器 adapters 以及对应的仿函数，完成一个简易的类标准库调用容器的测试过程，初步掌握了 Cmake 项目的创建，更细致理解了泛型编程的操作过程和使用原理。本项目具有大部分 STL 库的实现功能，比如 vector 动态数组的 push_back, emplace_back； list 双向循环链表的插入删除节点操作、顶堆 heap 隐式使用算法在 priority_queue 优先级队列的实现等。

- **项目补充**：(相比较现有的小型 MyTinySTL 项目)

  1.设计 slist 单向链表，为了更加贴合 SGI，本项目补充了这块内容。

  2.本项目在 SGI 的 list 内部排序(基数排序的变体)中，又额外添置了快速排序和归并排序两种排序算法的选择，并通过传入一个模板参数 Compared，判断其升序还是降序，默认升序。
  
  3.独立设计通用的 iteratoradapters、 functionadapters 本项目在 iterator.h 文件中添加并使用。
  
  4.补充并完善算法部分，如数值算法中的 power，基本算法中的 equal 等。
  
  5.使用 Gtest 编写测试用例，进行项目单元测试。

## Development environment
- Ubuntu 20.04.6
- gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.3) 
- cmake version 3.16.3
- Gtest(googletest-1.14.0)

*Recommendation for Useful Tools*：
- clangd-20(C++代码编写体验up up)
- zsh(类比bash, 但是有着更优雅的终端美化)
- apt-fast(快速更新安装工具)
- lldb(比gdb更舒适高效的命令行调试界面)

## Install and Usage
```
$ mkdir build && cd build
$ cmake .. #执行 cmake 生成 Makefile  cmake指令需要在和 CMakeLists.txt 文件同一个目录下执行 cmake之后我们发现在build文件夹下多了 Makefile 以及其他文件
$ make # 生成可执行文件
$ cd ../bin && ./stltest # 完整的单元测试，如果想查看单个的ut可在对应的CMakeLists.txt注释修改，具体查看该文件内容。

$ cd build
# 只删除生成的目标文件、缓存，但保留 CMakeCache.txt
$ cmake --build . --target clean

# 连 CMakeCache.txt 也清掉
$ cmake --fresh .
```
使用方式很简单，解压然后调用对应头文件即可，也可以生成对应的静态链接库进行使用。

本项目也提供直接测试的脚本，在项目下运行 build.py 即可。

## 

___注意：master分支是初步的完工版本；之后的bugfix_refactor是二次查阅项目的代码重构版本，相比原本只是在代码格式上有些许变化，后新增了对于基础的排序算法的代码实现。___