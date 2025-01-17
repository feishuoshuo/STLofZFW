#ifndef GOOGLETEST_SAMPLES_stack_queue_H_
#define GOOGLETEST_SAMPLES_stack_queue_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../STL/stack.h"
#include "../STL/queue.h"
/**
 * SContainerTestStack: 序列容器测试类
 * 测试类继承自 ::testing::Test，它将用于所有测试用例
 * -----------------------------------------------------
 */
void print_start()
{
  std::cout << "[===============================================================]\n";
  std::cout << "[----------------- Run container test : stack/queue -----------------]\n";
  std::cout << "[-------------------------- API test ---------------------------]\n";
}
void print_process(string tmp)
{
  std::cout << "[-------------------------- " << tmp << " ---------------------------]\n";
}
// 测试类
class SContainerTestStack : public ::testing::Test
{
protected:
  zfwstl::stack<int> v1, v2;

  void SetUp() override
  {
    v1 = {1, 2, 3, 4, 5};
    v2 = {6, 7, 8, 9, 10};
  }
};
class SContainerTestQueue : public ::testing::Test
{
protected:
  zfwstl::queue<int> v1, v2;

  void SetUp() override
  {
    v1 = {1, 2, 3, 4, 5};
    v2 = {6, 7, 8, 9, 10};
  }
};
//===============测试用例开始===============
// 测试构造函数
TEST_F(SContainerTestStack, Constructor)
{

  print_process("stack: Default constructor");
  zfwstl::stack<int> v3; // 默认构造
  EXPECT_TRUE(v3.empty());

  print_process("Fill constructor");
  zfwstl::stack<int> v4(5, 10); // 填充构造
  EXPECT_EQ(v4.size(), 5);

  print_process("Copy constructor");
  zfwstl::stack<int> v5(v1); // 拷贝构造
  EXPECT_EQ(v5, v1);
  EXPECT_FALSE(v1.empty());

  print_process("Move constructor");
  zfwstl::stack<int> v6(std::move(v2)); // 移动构造
  EXPECT_NE(v6, v2);
  EXPECT_TRUE(v2.empty());
}
TEST_F(SContainerTestQueue, Constructor)
{

  print_process("queue: Default constructor");
  zfwstl::queue<int> v3; // 默认构造
  EXPECT_TRUE(v3.empty());

  print_process("Fill constructor");
  zfwstl::queue<int> v4(5, 10); // 填充构造
  EXPECT_EQ(v4.size(), 5);

  print_process("Copy constructor");
  zfwstl::queue<int> v5(v1); // 拷贝构造
  EXPECT_EQ(v5, v1);
  EXPECT_FALSE(v1.empty());

  print_process("Move constructor");
  zfwstl::queue<int> v6(std::move(v2)); // 移动构造
  EXPECT_NE(v6, v2);
  EXPECT_TRUE(v2.empty());
}
// 测试operator=复制赋值操作符
TEST_F(SContainerTestStack, AssignmentOperator)
{
  print_process("stack: Assignment operator");
  zfwstl::stack<int> v3 = v1; // 赋值操作运算符
  EXPECT_EQ(v3, v1);

  print_process("initializer_list Assignment operator");
  zfwstl::stack<int> v4 = zfwstl::stack<int>({4, 5, 6});
  EXPECT_EQ(v4.size(), 3);
  EXPECT_EQ(v4.top(), 6);

  // 移动赋值操作运算符
  print_process("Move Assignment operator");
  zfwstl::stack<int> v5 = zfwstl::stack<int>(6, 6);
  EXPECT_EQ(v5.size(), 6);
}
TEST_F(SContainerTestQueue, AssignmentOperator)
{
  print_process("queue: Assignment operator");
  zfwstl::queue<int> v3 = v1; // 赋值操作运算符
  EXPECT_EQ(v3, v1);

  print_process("initializer_list Assignment operator");
  zfwstl::queue<int> v4 = zfwstl::queue<int>({4, 5, 6});
  EXPECT_EQ(v4.size(), 3);
  EXPECT_EQ(v4.front(), 4);
  EXPECT_EQ(v4.back(), 6);

  // 移动赋值操作运算符
  print_process("Move Assignment operator");
  zfwstl::queue<int> v5 = zfwstl::queue<int>(6, 6);
  EXPECT_EQ(v5.size(), 6);
  EXPECT_EQ(v5.front(), 6);
  EXPECT_EQ(v5.back(), 6);
}
// 测试 push_back, pop_back, emplace_back, emplace 方法
TEST_F(SContainerTestStack, EmplacePushPop)
{
  print_process("push");
  v1.push(6);
  EXPECT_EQ(v1.top(), 6);
  EXPECT_EQ(v1.size(), 6);

  print_process("pop");
  v1.pop();
  EXPECT_EQ(v1.top(), 5);
  EXPECT_EQ(v1.size(), 5);

  print_process("emplace");
  v1.emplace(8);
  EXPECT_EQ(v1.top(), 8);
  EXPECT_EQ(v1.size(), 6);
}
TEST_F(SContainerTestQueue, EmplacePushPop)
{
  print_process("push");
  v1.push(6);
  EXPECT_EQ(v1.back(), 6);
  EXPECT_EQ(v1.size(), 6);

  print_process("pop");
  v2.pop();
  EXPECT_EQ(v2.front(), 7);
  EXPECT_EQ(v2.back(), 10);
  EXPECT_EQ(v2.size(), 4);

  print_process("emplace");
  v1.emplace(8);
  EXPECT_EQ(v1.back(), 8);
  EXPECT_EQ(v1.size(), 7);
}
// 测试 clear 方法
TEST_F(SContainerTestStack, Clear)
{
  print_process("stack: clear & empty");
  v1.clear();
  EXPECT_TRUE(v1.empty());
}
TEST_F(SContainerTestQueue, Clear)
{
  print_process("queue: clear & empty");
  v1.clear();
  EXPECT_TRUE(v1.empty());
}
// 测试 swap 方法
TEST_F(SContainerTestStack, Swap)
{
  print_process("stack: Swap");
  zfwstl::stack<int> v3 = {11, 12, 13, 14, 15};
  v1.swap(v3);
  EXPECT_EQ(v1.size(), 5);
  EXPECT_EQ(v3.size(), 5);
  EXPECT_EQ(v1.top(), 15);
  EXPECT_EQ(v3.top(), 5);
}
TEST_F(SContainerTestQueue, Swap)
{
  print_process("queue: Swap");
  zfwstl::queue<int> v3 = {11, 12, 13, 14, 15};
  v1.swap(v3);
  EXPECT_EQ(v1.size(), 5);
  EXPECT_EQ(v3.size(), 5);
  EXPECT_EQ(v1.front(), 11);
  EXPECT_EQ(v3.front(), 1);
}
// 测试 front 和 back 方法
TEST_F(SContainerTestQueue, FrontBack)
{
  print_process("queue: front & back");
  EXPECT_EQ(v1.front(), 1);
  EXPECT_EQ(v1.back(), 5);
}
// 测试各种比较操作运算符
TEST_F(SContainerTestQueue, CompareOperator)
{
  EXPECT_TRUE(v1 != v2);
  EXPECT_FALSE(v1 == v2);
  EXPECT_TRUE(v1 <= v2);
  EXPECT_FALSE(v1 >= v2);
  EXPECT_TRUE(v1 < v2);
  EXPECT_FALSE(v1 > v2);
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_stack_queue_H_