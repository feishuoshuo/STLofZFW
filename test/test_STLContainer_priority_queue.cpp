#ifndef GOOGLETEST_SAMPLES_priority_queue_H_
#define GOOGLETEST_SAMPLES_priority_queue_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../STL/priority_queue.h"
#include "../STL/vector.h"
/**
 * SContainerTestPriQue: 序列容器测试类
 * 测试类继承自 ::testing::Test，它将用于所有测试用例
 * -----------------------------------------------------
 */
void print_start()
{
  std::cout << "[===============================================================]\n";
  std::cout << "[----------------- Run container test : priority queue -----------------]\n";
  std::cout << "[-------------------------- API test ---------------------------]\n";
}
void print_process(string tmp)
{
  std::cout << "[-------------------------- " << tmp << " ---------------------------]\n";
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
class SContainerTestPriQue : public ::testing::Test
{
protected:
  zfwstl::priority_queue<int> v1, v2;

  void SetUp() override
  {
    v1 = {1, 2, 3, 4, 5}; // 初始化向量
    v2 = {6, 7, 8, 9, 10};
  }
};
//===============测试用例开始===============
// 测试构造函数
TEST_F(SContainerTestPriQue, Constructor)
{
  print_process("Default constructor");
  zfwstl::priority_queue<int> v3; // 默认构造
  EXPECT_TRUE(v3.empty());

  print_process("Fill constructor");
  zfwstl::priority_queue<int> v4(5, 10); // 填充构造
  EXPECT_EQ(v4.size(), 5);

  print_process("Copy constructor");
  zfwstl::priority_queue<int> v5(v1); // 拷贝构造
  EXPECT_EQ(v5, v1);
  EXPECT_FALSE(v1.empty());

  print_process("Move constructor");
  zfwstl::priority_queue<int> v6(std::move(v2)); // 移动构造
  EXPECT_NE(v6, v2);
  EXPECT_TRUE(v2.empty());

  print_process("range constructor");
  zfwstl::vector<int> vv2 = {4, 8, 2, 3, 7};
  auto it1 = vv2.begin();
  auto it2 = vv2.end();
  zfwstl::priority_queue<int> v7(it1, it2); // 范围构造函数
  EXPECT_EQ(v7.top(), 8);
}
// 测试operator=复制赋值操作符
TEST_F(SContainerTestPriQue, AssignmentOperator)
{
  print_process("Assignment operator");
  zfwstl::priority_queue<int> v3 = v1; // 赋值操作运算符
  EXPECT_EQ(v3, v1);

  print_process("initializer_list Assignment operator");
  zfwstl::priority_queue<int> v4 = zfwstl::priority_queue<int>({4, 5, 6});
  EXPECT_EQ(v4.size(), 3);
  EXPECT_EQ(v4.top(), 6);

  // 移动赋值操作运算符
  print_process("Move Assignment operator");
  zfwstl::priority_queue<int> v5(6, 6);
  EXPECT_EQ(v5.size(), 6);
  EXPECT_EQ(v5.top(), 6);
}
// 测试各类插入
TEST_F(SContainerTestPriQue, PushAndTop)
{
  print_process("push & emplace & top");
  zfwstl::priority_queue<int> pq;
  pq.push(10);
  pq.push(zfwstl::move(5));
  pq.emplace(20);

  EXPECT_EQ(20, pq.top());
  pq.pop();
  EXPECT_EQ(10, pq.top());
  pq.pop();
  EXPECT_EQ(5, pq.top());
}
// 测试 clear 方法
TEST_F(SContainerTestPriQue, Clear)
{
  print_process("clear & empty");
  v1.clear();
  EXPECT_TRUE(v1.empty());
}
// 测试 swap 方法
TEST_F(SContainerTestPriQue, Swap)
{
  print_process("Swap");
  zfwstl::priority_queue<int> v3 = {11, 12, 13, 14, 15};
  v1.swap(v3);
  EXPECT_EQ(v1.size(), 5);
  EXPECT_EQ(v3.size(), 5);
  EXPECT_EQ(v1.top(), 15);
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_priority_queue_H_