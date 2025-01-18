#ifndef GOOGLETEST_SAMPLES_map_H_
#define GOOGLETEST_SAMPLES_map_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../STL_2/map.h"
/**
 * AContainerTestMap: 关联容器测试类
 * 测试类继承自 ::testing::Test，它将用于所有测试用例
 * -----------------------------------------------------
 * InitialState：测试向量在初始化后的状态。
 * PushBack：测试 push_back 方法是否正确增加了元素并更新了大小。
 * Insert：测试 insert 方法是否正确插入了元素，并检查了插入位置及其后元素的状态
 */
void print_start()
{
  std::cout << "[===============================================================]\n";
  std::cout << "[----------------- Run container test : map -----------------]\n";
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
  zfwstl::map<std::string, int>::const_iterator ite1 = con.begin();
  zfwstl::map<std::string, int>::const_iterator ite2 = con.end();
  for (; ite1 != ite2; ++ite1)
  {
    std::cout << (*ite1).first << "=" << (*ite1).second << " ";
  }
  std::cout << std::endl;
}
// 测试类
class AContainerTestMap : public ::testing::Test
{
protected:
  zfwstl::map<std::string, int> simap;

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
TEST_F(AContainerTestMap, Constructor)
{

  print_process("Default constructor");
  zfwstl::map<int, std::string> v3; // 默认构造
  EXPECT_TRUE(v3.empty());

  print_process("Fill constructor");
  // 使用循环来初始化map
  string arr[] = {"c1", "a2", "v4", "o8", "@10"};
  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
    v3[i] = arr[i];
  EXPECT_EQ(v3.size(), 5);
  for (const auto &pair : v3)
  {
    EXPECT_EQ(pair.second, 10) << "Value at key " << pair.first << " is not 10";
  }

  print_process("Copy constructor");
  zfwstl::map<int, std::string> v5(v3); // 拷贝构造
  EXPECT_EQ(v5, v3);
  EXPECT_FALSE(v3.empty());

  print_process("Move constructor");
  zfwstl::map<int, std::string> v6(std::move(v3)); // 移动构造
  EXPECT_NE(v6, v3);
  EXPECT_TRUE(v3.empty());

  print_process("range constructor");
  auto it1 = simap.begin();
  auto it2 = simap.end();
  zfwstl::map<std::string, int> v7(it1, it2); // 范围构造函数
  EXPECT_EQ(v7, simap);
}
// 测试operator=复制赋值操作符
TEST_F(AContainerTestMap, AssignmentOperator)
{
  print_process("Assignment operator");
  zfwstl::map<std::string, int> v3 = simap; // 赋值操作运算符
  EXPECT_EQ(v3, v1);

  print_process("initializer_list Assignment operator");
  zfwstl::map<std::string, int> v4 = zfwstl::map<std::string, int>({4, 5, 6});
  EXPECT_EQ(v4.size(), 3);
  EXPECT_EQ(v4.front(), 4);
  EXPECT_EQ(v4.back(), 6);

  // 移动赋值操作运算符
  print_process("Move Assignment operator");
  zfwstl::map<int> v5 = zfwstl::map<int>(6, 6);
  EXPECT_EQ(v5.size(), 6);
  EXPECT_EQ(v5.front(), 6);
  EXPECT_EQ(v5.back(), 6);
}
// 测试 assign 方法
TEST_F(AContainerTestMap, Assign)
{
  print_process("Fill assignment");
  v1.assign(3, 100); // 填充赋值
  EXPECT_EQ(v1.size(), 3);
  for (int i : v1)
  {
    EXPECT_EQ(i, 100);
  }

  print_process("Range assignment");
  int arr[] = {1, 2, 3, 4, 5};
  v1.assign(arr, arr + 5); // 范围赋值
  EXPECT_EQ(v1.size(), 5);
  for (size_t i = 0; i < v1.size(); ++i)
  {
    EXPECT_EQ(v1[i], arr[i]);
  }

  print_process("Initialize list assignment");
  v1.assign({9, 8, 7, 6, 10, 2, 1, 0, 4});
  EXPECT_EQ(v1.size(), 9);
}
// 测试 push_back, pop_back, emplace_back, emplace 方法
TEST_F(AContainerTestMap, EmplacePushPopBack)
{
  print_process("push_back");
  v1.push_back(6);
  EXPECT_EQ(v1.back(), 6);
  EXPECT_EQ(v1.size(), 6);

  print_process("pop_back");
  v1.pop_back();
  EXPECT_EQ(v1.back(), 5);
  EXPECT_EQ(v1.size(), 5);

  print_process("emplace_back");
  v1.emplace_back(6);
  EXPECT_EQ(v1.back(), 6);
  EXPECT_EQ(v1.size(), 6);

  print_process("emplace");
  auto it = v1.begin() + 3;
  auto it2 = v1.emplace(it, 10);
  EXPECT_EQ(*(it2), 10);
  EXPECT_EQ(v1.size(), 7);
}
// 测试 insert 方法
TEST_F(AContainerTestMap, Insert)
{
  print_process("Inserts a single element at the specified position");
  auto it = v1.begin() + 2;
  v1.insert(it, 100); // 在指定位置插入单个元素
  EXPECT_EQ(v1[2], 100);
  EXPECT_EQ(v1.size(), 6);

  print_process("Inserts more than one of the same elements at the specified location");
  it = v1.begin() + 2;
  v1.insert(it, 2, 200); // 在指定位置插入多个相同元素
  EXPECT_EQ(v1[2], 200);
  EXPECT_EQ(v1[3], 200);
  EXPECT_EQ(v1.size(), 8);
}
// 测试 erase 方法
TEST_F(AContainerTestMap, Erase)
{
  print_process("Erase a single element");
  auto it = v1.begin() + 2;
  v1.erase(it); // 删除单个元素
  EXPECT_EQ(v1.size(), 4);
  EXPECT_EQ(v1[2], 4);

  print_process("Erase the elements in the range");
  v1.erase(v1.begin(), v1.begin() + 2); // 删除范围内的元素
  EXPECT_EQ(v1.size(), 2);
  EXPECT_EQ(v1[0], 4);
  EXPECT_EQ(v1[1], 5);
}
// 测试 clear 方法
TEST_F(AContainerTestMap, Clear)
{
  print_process("clear & empty");
  v1.clear();
  EXPECT_TRUE(v1.empty());
}
// 测试 swap 方法
TEST_F(AContainerTestMap, Swap)
{
  print_process("Swap");
  zfwstl::map<int> v3 = {11, 12, 13, 14, 15};
  v1.swap(v3);
  EXPECT_EQ(v1.size(), 5);
  EXPECT_EQ(v3.size(), 5);
  for (size_t i = 0; i < 5; ++i)
  {
    EXPECT_EQ(v1[i], 11 + i);
    EXPECT_EQ(v3[i], 1 + i);
  }
}
// 测试 at 方法
TEST_F(AContainerTestMap, At)
{
  print_process("At");
  EXPECT_EQ(v1.at(0), 1);
  EXPECT_EQ(v1.at(4), 5);
  // 下面的代码将抛出 std::out_of_range 异常，因为索引超出了范围
  // EXPECT_THROW(v1.at(5), std::out_of_range);
}
// 测试 front 和 back 方法
TEST_F(AContainerTestMap, FrontBack)
{
  print_process("front & back");
  EXPECT_EQ(v1.front(), 1);
  EXPECT_EQ(v1.back(), 5);
}
// 测试数据成员 size 和 capacity
TEST_F(AContainerTestMap, SizeCapacity)
{
  print_process("size & capacity");
  EXPECT_EQ(v1.size(), 5);
  EXPECT_LE(v1.size(), v1.capacity());
}
// 测试数据成员 reserve(调整容量), resize(重置容器大小)
TEST_F(AContainerTestMap, ReserveResize)
{
  print_process("reserve & resize");
  EXPECT_EQ(v1.size(), 5);
  v1.reserve(20);
  EXPECT_EQ(v1.capacity(), 20);

  v1.resize(6);
  EXPECT_EQ(v1.size(), 6);
  EXPECT_EQ(v1.back(), 0);
  v1.resize(10, 3);
  EXPECT_EQ(v1.size(), 10);
}
// 测试一系列反向迭代器rbegin, rend()
TEST_F(AContainerTestMap, BeginEndIterators)
{
  print_process("rbegin/end & rcbegin/end");
  auto it = v2.begin();
  EXPECT_EQ(*it, 6); // 检查 begin() 是否指向第一个元素
  auto end_it = v2.end();
  EXPECT_EQ(*(--end_it), 10); // 检查 end() 前一个元素是否是最后一个元素

  auto rbegin_it = v2.rbegin();
  EXPECT_EQ(*rbegin_it, 10); // 检查 rbegin() 是否指向最后一个元素
  auto rend_it = v2.rend();
  EXPECT_EQ(*(--rend_it), 6); // 检查 rend() 前一个元素是否是第一个元素

  auto cbegin_it = v2.cbegin();
  EXPECT_EQ(*cbegin_it, 6); // 检查 cbegin() 是否指向第一个元素
  auto cend_it = v2.cend();
  EXPECT_EQ(*(--cend_it), 10); // 检查 cend() 前一个元素是否是最后一个元素

  auto crbegin_it = v2.crbegin();
  EXPECT_EQ(*crbegin_it, 10); // 检查 crbegin() 是否指向第一个元素
  auto crend_it = v2.crend();
  EXPECT_EQ(*(--crend_it), 6); // 检查 crend() 前一个元素是否是最后一个元素
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_map_H_