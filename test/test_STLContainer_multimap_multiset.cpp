#ifndef GOOGLETEST_SAMPLES_map_H_
#define GOOGLETEST_SAMPLES_map_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../STL_2/multimap.h"
#include "../STL_2/multiset.h"
#include "../src/util.h" // for make_pair
/**
 * AContainerTestMMap: 关联容器测试类
 * 测试类继承自 ::testing::Test，它将用于所有测试用例
 * -----------------------------------------------------
 * InitialState：测试向量在初始化后的状态。
 * PushBack：测试 push_back 方法是否正确增加了元素并更新了大小。
 * Insert：测试 insert 方法是否正确插入了元素，并检查了插入位置及其后元素的状态
 */
void print_start()
{
  std::cout << "[===============================================================]\n";
  std::cout << "[----------------- Run container test : multi_map & multi_set -----------------]\n";
  std::cout << "[-------------------------- API test ---------------------------]\n";
}
void print_process(string tmp)
{
  std::cout << "[---- " << tmp << " ----]\n";
}
// 测试类
class AContainerTestMMap : public ::testing::Test
{
protected:
  zfwstl::multimap<std::string, int> simap;
  void priMap()
  {
    print_process("map:");
    zfwstl::multimap<std::string, int>::iterator ite1 = simap.begin();
    zfwstl::multimap<std::string, int>::iterator ite2 = simap.end();

    for (; ite1 != ite2; ++ite1)
      std::cout << (*ite1).first << "=" << (*ite1).second << " ";
    std::cout << std::endl;
  }

  void SetUp() override
  {
    // 在每个测试用例执行前插入一些初始数据
    simap.insert({"one", 1});
    simap.insert({"two", 2});
    simap.insert({"three", 3});
  }
};
//===============测试用例开始===============
// 测试构造函数
TEST_F(AContainerTestMMap, Constructor)
{

  print_process("Default constructor");
  zfwstl::multimap<int, std::string> v3; // 默认构造
  EXPECT_TRUE(v3.empty());

  print_process("Fill constructor");
  // 使用循环来初始化map
  string arr[] = {"c1", "a2", "v4", "o8", "@10"};
  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
    v3[i] = arr[i];
  EXPECT_EQ(v3.size(), 5);
  int j = 0;
  for (const auto &pair : v3)
  {
    EXPECT_EQ(pair.first, j) << "key at value " << pair.second << std::endl;
    ++j;
  }

  print_process("Copy constructor");
  zfwstl::multimap<int, std::string> v5(v3); // 拷贝构造
  EXPECT_EQ(v5, v3);
  EXPECT_FALSE(v3.empty());

  print_process("Move constructor");
  zfwstl::multimap<int, std::string> v6(zfwstl::move(v3)); // 移动构造
  EXPECT_NE(v6, v3);
  EXPECT_TRUE(v3.empty());

  print_process("range constructor");
  auto it1 = simap.begin();
  auto it2 = simap.end();
  zfwstl::multimap<std::string, int> v7(it1, it2); // 范围构造函数
  EXPECT_EQ(v7, simap);

  print_process("initializer_list constructor"); // 列表初始化构造
  zfwstl::multimap<int, std::string> myMap({{1, "one"}, {2, "two"}, {3, "three"}});

  EXPECT_EQ((*(myMap.find(1))).second, "one");
  EXPECT_EQ((*(myMap.find(2))).second, "two");
  EXPECT_EQ((*(myMap.find(3))).second, "three");
}
// 测试operator=复制赋值操作符
TEST_F(AContainerTestMMap, AssignmentOperator)
{
  print_process("Assignment operator");
  zfwstl::multimap<std::string, int> v3 = simap; // 赋值操作运算符
  EXPECT_EQ(v3, simap);

  print_process("initializer_list Assignment operator");
  zfwstl::multimap<std::string, int> v4 = zfwstl::multimap<std::string, int>({{"four", 4},
                                                                              {"five", 5},
                                                                              {"six", 6}});
  EXPECT_EQ(v4.size(), 3);
  EXPECT_EQ((*(v4.find("four"))).second, 4);
  EXPECT_EQ((*(v4.find("five"))).second, 5);
  EXPECT_EQ((*(v4.find("six"))).second, 6);

  // 移动赋值操作运算符
  print_process("Move Assignment operator");
  zfwstl::multimap<std::string, int> v5 = zfwstl::move(simap);
  EXPECT_TRUE(simap.empty());
  EXPECT_EQ((*(v5.find("one"))).second, 1);
  EXPECT_EQ((*(v5.find("two"))).second, 2);
  EXPECT_EQ((*(v5.find("three"))).second, 3);
}
// 测试 insert, erase 方法
TEST_F(AContainerTestMMap, InsertErase)
{
  print_process("Inserts a single element at the specified position");
  auto res = simap.insert(zfwstl::make_pair(std::string("zhoufeiwei"), 100)); // 在指定位置插入单个元素
  auto i = (*(res)).second;
  EXPECT_EQ(i, 100);
  EXPECT_EQ(simap.size(), 4);

  print_process("Inserts more than one of the same elements at the specified location");
  zfwstl::multimap<std::string, int> v3({{"liuliuilu", 6}, {"egiht", 888}});
  simap.insert(v3.begin(), v3.end()); // 在指定位置插入多个相同元素
  EXPECT_EQ((*(simap.find("liuliuilu"))).second, 6);
  EXPECT_EQ((*(simap.find("egiht"))).second, 888);
  EXPECT_EQ(simap.size(), 6);

  print_process("Erase a single element");
  auto it = simap.begin();
  ++ ++it;
  priMap();
  simap.erase(it); // 删除单个元素
  EXPECT_EQ(simap.size(), 5);
  priMap();

  print_process("Erase the elements in the range");
  simap.erase(simap.begin(), ++ ++(simap.begin())); // 删除范围内的元素
  EXPECT_EQ(simap.size(), 3);
  priMap();
}
// 测试 clear 方法
TEST_F(AContainerTestMMap, Clear)
{
  print_process("clear & empty");
  simap.clear();
  EXPECT_TRUE(simap.empty());
}
// 测试 swap 方法
TEST_F(AContainerTestMMap, Swap)
{
  print_process("Swap");
  zfwstl::multimap<std::string, int> v3({{"liuliuilu", 6}});
  simap.swap(v3);
  EXPECT_EQ(simap.size(), 1);
  EXPECT_EQ(v3.size(), 3);
  EXPECT_NE((*(v3.find("liuliuilu"))).second, 6); // multimap键值可重复
  EXPECT_EQ((*(v3.find("one"))).second, 1);
}
TEST_F(AContainerTestMMap, LowerUpperBound)
{
  priMap();
  auto ite1 = simap.find(string("one"));
  EXPECT_TRUE(ite1 != simap.end());

  print_process("lower_bound");
  auto it = simap.lower_bound("one");
  EXPECT_TRUE(it != simap.end());
  EXPECT_EQ((*(it)).first, "one");

  it = simap.lower_bound("two");
  EXPECT_TRUE(it != simap.end());
  EXPECT_EQ((*(it)).first, "two");

  it = simap.lower_bound("three");
  EXPECT_TRUE(it != simap.end());
  EXPECT_EQ((*(it)).first, "three");

  print_process("upper_bound");
  it = simap.upper_bound("one");
  EXPECT_EQ((*(it)).second, 3);

  it = simap.upper_bound("two");
  EXPECT_TRUE(it == simap.end());

  it = simap.upper_bound("three");
  EXPECT_TRUE(it != simap.end());
  EXPECT_EQ((*(it)).second, 2);

  print_process("equal_range");
  simap.insert(zfwstl::make_pair(std::string("four"), 4));
  simap.insert(zfwstl::make_pair(std::string("five"), 5));
  simap.insert(zfwstl::make_pair(std::string("five"), 5));
  simap.insert(zfwstl::make_pair(std::string("six"), 6));
  // 测试存在的键
  auto range = simap.equal_range("five");
  EXPECT_EQ(simap.count("five"), 2);

  // 测试不存在的键
  range = simap.equal_range("zhoufeiwei");
  EXPECT_EQ(simap.count("zhoufeiwei"), 0);
}
// 测试一系列反向迭代器rbegin, rend()
TEST_F(AContainerTestMMap, BeginEndIterators)
{
  print_process("rbegin/end & rcbegin/end");
  auto it = simap.begin();
  EXPECT_EQ((*(it)).first, "one"); // 检查 begin() 是否指向第一个元素
  auto end_it = simap.end();
  EXPECT_EQ((*(--end_it)).first, "two"); // 检查 end() 前一个元素是否是最后一个元素

  auto rbegin_it = simap.rbegin();
  EXPECT_EQ((*(rbegin_it)).first, "two"); // 检查 rbegin() 是否指向最后一个元素
  auto rend_it = simap.rend();
  EXPECT_EQ((*(--rend_it)).first, "one"); // 检查 rend() 前一个元素是否是第一个元素

  auto cbegin_it = simap.cbegin();
  EXPECT_EQ((*(cbegin_it)).first, "one"); // 检查 cbegin() 是否指向第一个元素
  auto cend_it = simap.cend();
  EXPECT_EQ((*(--cend_it)).first, "two"); // 检查 cend() 前一个元素是否是最后一个元素

  auto crbegin_it = simap.crbegin();
  EXPECT_EQ((*(crbegin_it)).first, "two"); // 检查 crbegin() 是否指向第一个元素
  auto crend_it = simap.crend();
  EXPECT_EQ((*(--crend_it)).first, "one"); // 检查 crend() 前一个元素是否是最后一个元素
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_map_H_