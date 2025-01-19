#ifndef GOOGLETEST_SAMPLES_map_H_
#define GOOGLETEST_SAMPLES_map_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../STL_2/unordered_map.h"
#include "../src/util.h" // for make_pair, move
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
  std::cout << "[----------------- Run container test : unordered_map -----------------]\n";
  std::cout << "[-------------------------- API test ---------------------------]\n";
}
void print_process(string tmp)
{
  std::cout << "[---- " << tmp << " ----]\n";
}
template <class Container>
void pri_element(Container &con, std::string name)
{
  auto ite1 = con.begin();
  auto ite2 = con.end();
  std::cout << name << std::endl;
  for (; ite1 != ite2; ++ite1)
    std::cout << (*ite1).first << "=" << (*ite1).second << " ";
  std::cout << std::endl;
}
// 测试类
class AContainerTestUMap : public ::testing::Test
{
protected:
  zfwstl::unordered_map<const char *, int> simap;
  void priMap()
  {
    print_process("unordered_map:");
    zfwstl::unordered_map<const char *, int>::iterator ite1 = simap.begin();
    zfwstl::unordered_map<const char *, int>::iterator ite2 = simap.end();

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
TEST_F(AContainerTestUMap, Constructor)
{

  print_process("Default constructor");
  zfwstl::unordered_map<int, const char *> v3; // 默认构造
  EXPECT_TRUE(v3.empty());

  print_process("Fill constructor");
  // 使用循环来初始化map
  const char *arr[] = {"c1", "a2", "v4", "o8", "@10"};
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
  zfwstl::unordered_map<int, const char *> v5(v3); // 拷贝构造
  EXPECT_EQ(v5, v3);
  EXPECT_FALSE(v3.empty());

  print_process("Move constructor");
  zfwstl::unordered_map<int, const char *> v6(zfwstl::move(v3)); // 移动构造
  EXPECT_NE(v6, v3);
  EXPECT_TRUE(v3.empty());

  print_process("range constructor");
  auto it1 = simap.begin();
  auto it2 = simap.end();
  zfwstl::unordered_map<const char *, int> v7(it1, it2); // 范围构造函数
  EXPECT_EQ(v7, simap);

  print_process("initializer_list constructor"); // 列表初始化构造
  zfwstl::unordered_map<int, const char *> myMap({{1, "one"}, {2, "two"}, {3, "three"}});
  EXPECT_EQ(myMap[1], "one");
  EXPECT_EQ(myMap[2], "two");
  EXPECT_EQ(myMap[3], "three");
}
// 测试operator=复制赋值操作符
TEST_F(AContainerTestUMap, AssignmentOperator)
{
  print_process("Assignment operator");
  zfwstl::unordered_map<const char *, int> v3 = simap; // 赋值操作运算符
  EXPECT_EQ(v3, simap);

  print_process("initializer_list Assignment operator");
  zfwstl::unordered_map<const char *, int> v4 = zfwstl::unordered_map<const char *, int>({{"four", 4},
                                                                                          {"five", 5},
                                                                                          {"six", 6}});
  EXPECT_EQ(v4.size(), 3);
  EXPECT_EQ(v4["four"], 4);
  EXPECT_EQ(v4["five"], 5);
  EXPECT_EQ(v4["six"], 6);

  // 移动赋值操作运算符
  print_process("Move Assignment operator");
  zfwstl::unordered_map<const char *, int> v5 = zfwstl::move(simap);
  EXPECT_TRUE(simap.empty());
  EXPECT_EQ(v5["one"], 1);
  EXPECT_EQ(v5["two"], 2);
  EXPECT_EQ(v5["three"], 3);
}
// 测试 insert, erase 方法
TEST_F(AContainerTestUMap, InsertErase)
{
  print_process("Inserts a single element at the specified position");
  auto res = simap.insert(zfwstl::make_pair("zhoufeiwei", 100)); // 在指定位置插入单个元素
  auto i = (*(res)).second;
  EXPECT_EQ(i, 100);
  EXPECT_EQ(simap.size(), 4);

  print_process("Inserts more than one of the same elements at the specified location");
  zfwstl::unordered_map<const char *, int> v3({{"liuliuilu", 6}, {"egiht", 888}});
  simap.insert(v3.begin(), v3.end()); // 在指定位置插入多个相同元素
  EXPECT_EQ(simap["liuliuilu"], 6);
  EXPECT_EQ(simap["egiht"], 888);
  EXPECT_EQ(simap.size(), 6);

  print_process("Erase a single element");
  auto it = simap.begin();
  ++ ++it;
  pri_element(simap, "simap");
  simap.erase(it); // 删除单个元素
  pri_element(simap, "simap");
  EXPECT_EQ(simap.size(), 5);

  print_process("Erase the elements in the range");
  simap.erase(simap.begin(), ++ ++(simap.begin())); // 删除范围内的元素
  EXPECT_EQ(simap.size(), 3);
  priMap();
}
// 测试 clear 方法
TEST_F(AContainerTestUMap, Clear)
{
  print_process("clear & empty");
  simap.clear();
  EXPECT_TRUE(simap.empty());
}
// 测试 swap 方法
TEST_F(AContainerTestUMap, Swap)
{
  print_process("Swap");
  zfwstl::unordered_map<const char *, int> v3({{"liuliuilu", 6}});
  simap.swap(v3);
  EXPECT_EQ(simap.size(), 1);
  EXPECT_EQ(v3.size(), 3);
  EXPECT_EQ(simap["liuliuilu"], 6);
  EXPECT_EQ(v3["one"], 1);
}
// 测试一系列迭代器
TEST_F(AContainerTestUMap, BeginEndIterators)
{
  print_process("rbegin/end");
  auto it = simap.begin();
  EXPECT_EQ((*(it)).first, "one"); // 检查 begin() 是否指向第一个元素

  auto cbegin_it = simap.cbegin();
  EXPECT_EQ((*(cbegin_it)).first, "one"); // 检查 cbegin() 是否指向第一个元素
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_map_H_