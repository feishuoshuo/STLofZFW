#ifndef GOOGLETEST_SAMPLES_list_H_
#define GOOGLETEST_SAMPLES_list_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../src/iterator.h" //for advance
#include "../STL/slist.h"
/**
 * SContainerTestList: 序列容器测试类
 * 测试类继承自 ::testing::Test，它将用于所有测试用例
 * -----------------------------------------------------
 * InitialState：测试向量在初始化后的状态。
 * PushBack：测试 push_back 方法是否正确增加了元素并更新了大小。
 * Insert：测试 insert 方法是否正确插入了元素，并检查了插入位置及其后元素的状态
 */
void print_start()
{
  std::cout << "[===============================================================]\n";
  std::cout << "[----------------- Run container test : slist -----------------]\n";
  std::cout << "[-------------------------- API test ---------------------------]\n";
}
void print_process(string tmp)
{
  std::cout << "[---- " << tmp << " ----]\n";
}
template <class Container>
void print_element(const Container &con, const std::string &name)
{
  std::cout << name << " contains: ";
  for (const auto &elem : con)
  {
    std::cout << elem << " ";
  }
  std::cout << std::endl;
}
// 测试类
class SContainerTestSList : public ::testing::Test
{
protected:
  zfwstl::slist<int> v1, v2;

  void SetUp() override
  {
    v1 = {1, 2, 3, 4, 5};
    v2 = {6, 7, 8, 9, 10};
  }
};
//===============测试用例开始===============
// 测试构造函数
TEST_F(SContainerTestSList, Constructor)
{

  print_process("Default constructor");
  zfwstl::slist<int> v3; // 默认构造
  EXPECT_TRUE(v3.empty());

  print_process("Fill constructor");
  zfwstl::slist<int> v4(5, 10); // 填充构造
  EXPECT_EQ(v4.size(), 5);
  for (int i : v4)
  {
    EXPECT_EQ(i, 10);
  }

  print_process("Copy constructor");
  zfwstl::slist<int> v5(v1); // 拷贝构造
  EXPECT_EQ(v5, v1);
  EXPECT_FALSE(v1.empty());

  print_process("Move constructor");
  zfwstl::slist<int> v6(std::move(v2)); // 移动构造
  EXPECT_NE(v6, v2);
  EXPECT_TRUE(v2.empty());

  print_process("range constructor");
  auto it1 = v1.begin();
  auto it2 = v1.end();
  zfwstl::slist<int> v7(it1, it2); // 范围构造函数
  EXPECT_EQ(v7, v1);
}
// 测试operator=复制赋值操作符
TEST_F(SContainerTestSList, AssignmentOperator)
{
  print_process("Assignment operator");
  zfwstl::slist<int> v3 = v1; // 赋值操作运算符
  EXPECT_EQ(v3, v1);

  print_process("initializer_list Assignment operator");
  zfwstl::slist<int> v4 = zfwstl::slist<int>({4, 5, 6});
  EXPECT_EQ(v4.size(), 3);
  EXPECT_EQ(v4.front(), 4);

  // 移动赋值操作运算符
  print_process("Move Assignment operator");
  zfwstl::slist<int> v5 = zfwstl::slist<int>(6, 6);
  EXPECT_EQ(v5.size(), 6);
  EXPECT_EQ(v5.front(), 6);
}
// 测试 clear 方法
TEST_F(SContainerTestSList, Clear)
{
  print_process("clear & empty");
  v1.clear();
  EXPECT_TRUE(v1.empty());
}
// 测试 swap 方法
TEST_F(SContainerTestSList, Swap)
{
  print_process("Swap");
  zfwstl::slist<int> v3 = {11, 12, 13, 14, 15};
  v1.swap(v3);
  EXPECT_EQ(v1.size(), 5);
  EXPECT_EQ(v3.size(), 5);
  int x = 0;
  for (auto i = v1.begin(), j = v3.begin(); i != v1.end() && j != v3.end(); ++i, ++j, ++x)
  {
    EXPECT_EQ(*i, 11 + x);
    EXPECT_EQ(*j, 1 + x);
  }
}
// 测试插入节点和删除节点
TEST_F(SContainerTestSList, PushPopFront)
{
  print_process("push_front");
  zfwstl::slist<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  zfwstl::slist<int>::iterator it = list.begin();
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 1);

  print_process("push_back");
  zfwstl::slist<int> list2;
  list2.push_back(1);
  list2.push_back(2);
  list2.push_back(3);
  auto it2 = list2.begin();
  EXPECT_EQ(*it2, 1);
  ++it2;
  EXPECT_EQ(*it2, 2);
  ++it2;
  EXPECT_EQ(*it2, 3);
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_list_H_