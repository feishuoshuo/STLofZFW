#ifndef GOOGLETEST_SAMPLES_list_H_
#define GOOGLETEST_SAMPLES_list_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../src/iterator.h" //for advance
#include "../STL/list.h"
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
  std::cout << "[----------------- Run container test : list -----------------]\n";
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
class SContainerTestList : public ::testing::Test
{
protected:
  zfwstl::list<int> v1, v2;

  void SetUp() override
  {
    v1 = {1, 2, 3, 4, 5}; // 初始化向量
    v2 = {6, 7, 8, 9, 10};
  }
};
//===============测试用例开始===============
// 测试构造函数
TEST_F(SContainerTestList, Constructor)
{

  print_process("Default constructor");
  zfwstl::list<int> v3; // 默认构造
  EXPECT_TRUE(v3.empty());

  print_process("Fill constructor");
  zfwstl::list<int> v4(5, 10); // 填充构造
  EXPECT_EQ(v4.size(), 5);
  for (int i : v4)
  {
    EXPECT_EQ(i, 10);
  }

  print_process("Copy constructor");
  zfwstl::list<int> v5(v1); // 拷贝构造
  EXPECT_EQ(v5, v1);
  EXPECT_FALSE(v1.empty());

  print_process("Move constructor");
  zfwstl::list<int> v6(std::move(v2)); // 移动构造
  EXPECT_NE(v6, v2);
  EXPECT_TRUE(v2.empty());

  print_process("range constructor");
  auto it1 = v1.begin();
  auto it2 = v1.end();
  zfwstl::list<int> v7(it1, it2); // 范围构造函数
  EXPECT_EQ(v7, v1);
}
// 测试operator=复制赋值操作符
TEST_F(SContainerTestList, AssignmentOperator)
{
  print_process("Assignment operator");
  zfwstl::list<int> v3 = v1; // 赋值操作运算符
  EXPECT_EQ(v3, v1);

  print_process("initializer_list Assignment operator");
  zfwstl::list<int> v4 = zfwstl::list<int>({4, 5, 6});
  EXPECT_EQ(v4.size(), 3);
  EXPECT_EQ(v4.front(), 4);
  EXPECT_EQ(v4.back(), 6);

  // 移动赋值操作运算符
  print_process("Move Assignment operator");
  zfwstl::list<int> v5 = zfwstl::list<int>(6, 6);
  EXPECT_EQ(v5.size(), 6);
  EXPECT_EQ(v5.front(), 6);
  EXPECT_EQ(v5.back(), 6);
}
// 测试 assign 方法
TEST_F(SContainerTestList, Assign)
{
  print_process("Fill assignment");
  v1.assign(3, 100); // 填充赋值
  EXPECT_EQ(v1.size(), 3);
  for (int i : v1)
  {
    EXPECT_EQ(i, 100);
  }

  print_process("Range assignment");
  int arr[] = {9, 8, 7, 6, 10, 11};
  v1.assign(arr, arr + 6); // 范围赋值
  EXPECT_EQ(v1.size(), 6);
  auto it = v1.begin(); // 获取列表的开始迭代器
  for (size_t i = 0; i < v1.size(); ++i, ++it)
  {
    EXPECT_EQ(*it, arr[i]); // 使用迭代器访问元素
  }

  print_process("Initialize list assignment");
  v1.assign({9, 8, 7, 6, 10, 2, 1, 0, 4});
  EXPECT_EQ(v1.size(), 9);
}
// 测试 push_back, pop_back, emplace_back, emplace 方法
TEST_F(SContainerTestList, EmplacePushPopBack)
{
  print_process("push_back");
  v1.push_back(6);
  EXPECT_EQ(v1.back(), 6);
  EXPECT_EQ(v1.size(), 6);

  print_process("pop_back");
  v1.pop_back();
  EXPECT_EQ(v1.back(), 5);
  EXPECT_EQ(v1.size(), 5);

  print_process("emplace_front");
  v1.emplace_front(66);
  EXPECT_EQ(v1.front(), 66);
  EXPECT_EQ(v1.size(), 6);

  print_process("emplace_back");
  v1.emplace_back(6);
  EXPECT_EQ(v1.back(), 6);
  EXPECT_EQ(v1.size(), 7);

  print_process("emplace");
  auto it = v1.begin();
  zfwstl::advance(it, 3);
  auto it2 = v1.emplace(it, 10);
  EXPECT_EQ(*(it2), 10);
  EXPECT_EQ(v1.size(), 8);
}
// 测试 insert 方法
TEST_F(SContainerTestList, Insert)
{
  print_process("Inserts a single element at the specified position");
  auto it = v1.begin();
  zfwstl::advance(it, 2);
  v1.insert(it, 100); // 在指定位置插入单个元素
  it = v1.begin();
  zfwstl::advance(it, 2);
  EXPECT_EQ(*it, 100);
  EXPECT_EQ(v1.size(), 6);

  print_process("Inserts more than one of the same elements at the specified location");
  it = v1.begin();
  zfwstl::advance(it, 2);
  v1.insert(it, 2, 200); // 在指定位置插入多个相同元素
  it = v1.begin();
  zfwstl::advance(it, 2);
  EXPECT_EQ(*it, 200);
  it = v1.begin();
  zfwstl::advance(it, 3);
  EXPECT_EQ(*it, 200);
  EXPECT_EQ(v1.size(), 8);

  print_process("Use iterators to specify range inserts");
  int arr[] = {1, 2, 3};
  it = v2.begin();
  zfwstl::advance(it, 3);
  v2.insert(it, arr, arr + 3); // 使用迭代器指定范围插入
  EXPECT_EQ(v2.size(), 8);
  it = v2.begin();
  zfwstl::advance(it, 3);
  for (size_t i = 0; i < 3; ++i, ++it)
  {
    EXPECT_EQ(*it, arr[i]); // 使用迭代器访问元素
  }
}
// 测试 erase 方法
TEST_F(SContainerTestList, Erase)
{
  print_process("Erase a single element");
  auto it = v2.begin();
  zfwstl::advance(it, 2);
  v2.erase(it); // 删除单个元素
  EXPECT_EQ(v2.size(), 4);
  it = v2.begin();
  zfwstl::advance(it, 2);
  EXPECT_EQ(*it, 9);

  print_process("Erase the elements in the range");
  it = v1.begin();
  zfwstl::advance(it, 3);
  v1.erase(v1.begin(), it); // 删除范围内的元素
  EXPECT_EQ(v1.size(), 2);
  it = v1.begin();
  EXPECT_EQ(*it, 4);
  zfwstl::advance(it, 1);
  EXPECT_EQ(*it, 5);
}
// 测试 clear 方法
TEST_F(SContainerTestList, Clear)
{
  print_process("clear & empty");
  v1.clear();
  EXPECT_TRUE(v1.empty());
}
// 测试 swap 方法
TEST_F(SContainerTestList, Swap)
{
  print_process("Swap");
  zfwstl::list<int> v3 = {11, 12, 13, 14, 15};
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
// 测试 front 和 back 方法
TEST_F(SContainerTestList, FrontBack)
{
  print_process("front & back");
  EXPECT_EQ(v1.front(), 1);
  EXPECT_EQ(v1.back(), 5);
}
// 测试一系列反向迭代器rbegin, rend()
TEST_F(SContainerTestList, BeginEndIterators)
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
//===========测试 list独有的功能=================
// 测试splice拼接功能
TEST_F(SContainerTestList, Splice)
{
  print_process("Splice second list before 2 in first list");
  zfwstl::list<int> first = {1, 2, 3};
  zfwstl::list<int> second = {4, 5, 6};
  auto it = first.begin();
  ++it;
  first.splice(it, second);
  EXPECT_EQ(first, zfwstl::list<int>({1, 4, 5, 6, 2, 3}));

  print_process("Splice one element from second to first");
  zfwstl::list<int> first2 = {1, 2, 3};
  zfwstl::list<int> second2 = {4, 5};
  it = second2.begin();
  first2.splice(first2.end(), second2, it);
  EXPECT_EQ(first2, zfwstl::list<int>({1, 2, 3, 4}));
  EXPECT_EQ(second2, zfwstl::list<int>({5}));

  print_process("Take another list range and concatenate it");
  zfwstl::list<int> first3 = {1, 2, 3};
  zfwstl::list<int> second3 = {4, 5, 6};
  first3.splice(first3.end(), second3, second3.begin(), second3.end());
  EXPECT_EQ(first3, zfwstl::list<int>({1, 2, 3, 4, 5, 6}));
}
// 测试remove/unique移除元素值满足条件的元素
TEST_F(SContainerTestList, RemoveUnique)
{
  print_process("remove value");
  v1.remove(2);
  EXPECT_EQ(v1, zfwstl::list<int>({1, 3, 4, 5}));

  print_process("Removes elements that satisfy the condition");
  v2.remove_if([](int value)
               { return value % 2 == 0; });
  EXPECT_EQ(v2, zfwstl::list<int>({7, 9}));

  print_process("unique: Remove duplicate elements(Ordered sort)");
  zfwstl::list<int> v3 = {1, 1, 1, 3, 3, 4, 4, 10, 11, 11};
  v3.unique();
  EXPECT_EQ(v3, zfwstl::list<int>({1, 3, 4, 10, 11}));

  print_process("unique: Remove duplicate elements from the list that satisfy pred as true(Ordered sort)");
  zfwstl::list<int> lst = {1, 2, 1, 3, 4, 5, 5};
  lst.unique([](int a, int b)
             { return a >= b; });
  EXPECT_EQ(lst, zfwstl::list<int>({1, 2, 3, 4, 5}));
}
// 测试合并merge
TEST_F(SContainerTestList, Merge)
{
  print_process("merge");
  v1.merge(v2);
  EXPECT_EQ(v1, zfwstl::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));

  zfwstl::list<int> first2 = {1, 3, 5};
  zfwstl::list<int> second2 = {2, 4, 6};
  first2.merge(second2, [](int a, int b)
               { return a < b; });
  EXPECT_EQ(first2, zfwstl::list<int>({1, 2, 3, 4, 5, 6}));
}
// 测试reverse反转
TEST_F(SContainerTestList, Reverse)
{
  print_process("reverse");
  v2.reverse();
  EXPECT_EQ(v2, zfwstl::list<int>({10, 9, 8, 7, 6}));
}
// 测试各种排序sort
TEST_F(SContainerTestList, Sort)
{
  print_process("sort=SGI version");
  zfwstl::list<int> lst = {5, 2, 8, 3, 1, 6};
  lst.sort();
  EXPECT_EQ(lst, zfwstl::list<int>({1, 2, 3, 5, 6, 8}));

  print_process("sort=quick_sort");
  lst = {5, 2, 8, 3, 1, 6};
  lst.sort('q');
  EXPECT_EQ(lst, zfwstl::list<int>({1, 2, 3, 5, 6, 8}));

  print_process("sort=merge_sort");
  lst = {5, 2, 8, 3, 1, 6};
  lst.sort('m');
  EXPECT_EQ(lst, zfwstl::list<int>({1, 2, 3, 5, 6, 8}));

  zfwstl::list<int> lst2 = {5, 2, 8, 3, 1, 6};
  lst2.sort([](int a, int b)
            { return a < b; }, '\0');
  EXPECT_EQ(lst2, zfwstl::list<int>({1, 2, 3, 5, 6, 8}));
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_list_H_