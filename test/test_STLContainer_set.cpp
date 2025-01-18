#ifndef GOOGLETEST_SAMPLES_map_H_
#define GOOGLETEST_SAMPLES_map_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../STL_2/set.h"
#include "../src/util.h" // for make_pair
/**
 * AContainerTestSet: 关联容器测试类
 * 测试类继承自 ::testing::Test，它将用于所有测试用例
 * -----------------------------------------------------
 * InitialState：测试向量在初始化后的状态。
 * PushBack：测试 push_back 方法是否正确增加了元素并更新了大小。
 * Insert：测试 insert 方法是否正确插入了元素，并检查了插入位置及其后元素的状态
 */
void print_start()
{
  std::cout << "[===============================================================]\n";
  std::cout << "[----------------- Run container test : set -----------------]\n";
  std::cout << "[-------------------------- API test ---------------------------]\n";
}
void print_process(string tmp)
{
  std::cout << "[---- " << tmp << " ----]\n";
}
template <class Container>
void print_element(const Container &con, const std::string &name)
{
  std::cout << name << " ";
  auto ite1 = con.begin();
  auto ite2 = con.end();

  for (; ite1 != ite2; ++ite1)
    std::cout << (*ite1) << " ";
  std::cout << std::endl;
}
// 测试类
class AContainerTestSet : public ::testing::Test
{
protected:
  zfwstl::set<int> siset;
  void priSet()
  {
    print_process("set:");
    zfwstl::set<int>::iterator ite1 = siset.begin();
    zfwstl::set<int>::iterator ite2 = siset.end();

    for (; ite1 != ite2; ++ite1)
      std::cout << (*ite1) << " ";
    std::cout << std::endl;
  }

  void SetUp() override
  {
    // 在每个测试用例执行前插入一些初始数据
    siset.insert(1);
    siset.insert(2);
    siset.insert(3);
  }
};
//===============测试用例开始===============
// 测试构造函数
TEST_F(AContainerTestSet, Constructor)
{

  print_process("Default constructor");
  zfwstl::set<int> v3; // 默认构造
  EXPECT_TRUE(v3.empty());

  print_process("Fill constructor");
  // 使用循环来初始化map
  string arr[] = {"c1", "a2", "v4", "o8", "@10"};
  zfwstl::set<std::string> v4{arr, arr + 5};
  EXPECT_EQ(v4.size(), 5);
  print_element(v4, "v4");

  print_process("Copy constructor");
  zfwstl::set<std::string> v5(v4); // 拷贝构造
  EXPECT_EQ(v5, v4);
  EXPECT_FALSE(v4.empty());

  print_process("Move constructor");
  zfwstl::set<std::string> v6(zfwstl::move(v4)); // 移动构造
  EXPECT_NE(v6, v4);
  EXPECT_TRUE(v4.empty());

  print_process("range constructor");
  auto it1 = siset.begin();
  auto it2 = siset.end();
  zfwstl::set<int> v7(it1, it2); // 范围构造函数
  EXPECT_EQ(v7, siset);

  print_process("initializer_list constructor"); // 列表初始化构造
  zfwstl::set<std::string> mySet({"one", "two", "three"});
  EXPECT_EQ(mySet.size(), 3);
}
// 测试operator=复制赋值操作符
TEST_F(AContainerTestSet, AssignmentOperator)
{
  print_process("Assignment operator");
  zfwstl::set<int> v3 = siset; // 赋值操作运算符
  EXPECT_EQ(v3, siset);

  print_process("initializer_list Assignment operator");
  zfwstl::set<int> v4 = {1, 32, 7, 4};
  EXPECT_EQ(v4.size(), 4);

  // 移动赋值操作运算符
  print_process("Move Assignment operator");
  zfwstl::set<int> v5 = zfwstl::move(siset);
  EXPECT_TRUE(siset.empty());
  EXPECT_NE(v5, siset);
}
// 测试 insert, erase 方法
TEST_F(AContainerTestSet, InsertErase)
{
  print_process("Inserts a single element at the specified position");
  auto res = siset.insert(100); // 在指定位置插入单个元素
  EXPECT_EQ(*(res.first), 100);
  EXPECT_EQ(siset.size(), 4);

  print_process("Inserts more than one of the same elements at the specified location");
  zfwstl::set<int> v3({6, 888});
  siset.insert(v3.begin(), v3.end()); // 在指定位置插入多个相同元素
  EXPECT_EQ(siset.size(), 6);

  print_process("Erase a single element");
  auto it = siset.begin();
  ++ ++it;
  priSet();
  siset.erase(it); // 删除单个元素
  EXPECT_EQ(siset.size(), 5);
  priSet();

  print_process("Erase the elements in the range");
  siset.erase(siset.begin(), ++ ++(siset.begin())); // 删除范围内的元素
  EXPECT_EQ(siset.size(), 3);
  priSet();
}
// 测试 clear 方法
TEST_F(AContainerTestSet, Clear)
{
  print_process("clear & empty");
  siset.clear();
  EXPECT_TRUE(siset.empty());
}
// 测试 swap 方法
TEST_F(AContainerTestSet, Swap)
{
  print_process("Swap");
  zfwstl::set<int> v3({21, 54, 265, 987});
  siset.swap(v3);
  EXPECT_EQ(siset.size(), 4);
  EXPECT_EQ(v3.size(), 3);
}
TEST_F(AContainerTestSet, LowerUpperBound)
{
  print_process("lower_bound");

  auto it = siset.lower_bound(14);
  EXPECT_TRUE(it == siset.end());

  it = siset.lower_bound(2);
  EXPECT_TRUE(it != siset.end());
  EXPECT_EQ(*it, 2);

  it = siset.lower_bound(0);
  EXPECT_TRUE(it != siset.end());
  EXPECT_EQ(*it, 1);

  print_process("upper_bound");
  it = siset.upper_bound(2);
  EXPECT_EQ(*it, 3);

  it = siset.upper_bound(1);
  EXPECT_TRUE(it != siset.end());
  EXPECT_EQ(*it, 2);

  it = siset.upper_bound(3);
  ASSERT_TRUE(it == siset.end());

  print_process("equal_range");
  siset.insert(15);
  siset.insert(16);
  siset.insert(15);
  siset.insert(17);
  // 测试存在的键
  auto range = siset.equal_range(15);
  EXPECT_EQ(siset.count(15), 1);

  // 测试不存在的键
  range = siset.equal_range(99);
  EXPECT_EQ(siset.count(99), 0);
}
// 测试一系列反向迭代器rbegin, rend()
TEST_F(AContainerTestSet, BeginEndIterators)
{
  print_process("rbegin/end & rcbegin/end");
  auto it = siset.begin();
  EXPECT_EQ(*it, 1); // 检查 begin() 是否指向第一个元素
  auto end_it = siset.end();
  EXPECT_EQ(*(--end_it), 3); // 检查 end() 前一个元素是否是最后一个元素

  auto rbegin_it = siset.rbegin();
  EXPECT_EQ(*rbegin_it, 3); // 检查 rbegin() 是否指向最后一个元素
  auto rend_it = siset.rend();
  EXPECT_EQ(*(--rend_it), 1); // 检查 rend() 前一个元素是否是第一个元素

  auto cbegin_it = siset.cbegin();
  EXPECT_EQ(*cbegin_it, 1); // 检查 cbegin() 是否指向第一个元素
  auto cend_it = siset.cend();
  EXPECT_EQ(*(--cend_it), 3); // 检查 cend() 前一个元素是否是最后一个元素

  auto crbegin_it = siset.crbegin();
  EXPECT_EQ(*crbegin_it, 3); // 检查 crbegin() 是否指向第一个元素
  auto crend_it = siset.crend();
  EXPECT_EQ(*(--crend_it), 1); // 检查 crend() 前一个元素是否是最后一个元素
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_map_H_