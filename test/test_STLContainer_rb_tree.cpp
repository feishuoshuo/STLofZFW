#ifndef GOOGLETEST_SAMPLES_vector_H_
#define GOOGLETEST_SAMPLES_vector_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../STL_2/rb_tree.h"
#include "../src/functional.h" //for less
#include "../src/util.h"       //for make_pair
#include "../src/iterator.h"   //for distance()
/**
 * SContainerTestRBTree: 序列容器测试类
 * 测试类继承自 ::testing::Test，它将用于所有测试用例
 * -----------------------------------------------------
 * InitialState：测试向量在初始化后的状态。
 * PushBack：测试 push_back 方法是否正确增加了元素并更新了大小。
 * Insert：测试 insert 方法是否正确插入了元素，并检查了插入位置及其后元素的状态
 */
void print_start()
{
  std::cout << "[===============================================================]\n";
  std::cout << "[----------------- Run container test : rbTree -----------------]\n";
  std::cout << "[-------------------------- API test ---------------------------]\n";
}
void print_process(string tmp)
{
  std::cout << "[---- " << tmp << " ----]\n";
}
// KeyOfValue 函数对象(定义一个从值类型中提取键的函数对象)
template <class Key>
struct KeyOfValue
{
  const Key &operator()(const Key &val) const
  {
    return val;
  }
};
// 测试类
class RBTreeTest : public ::testing::Test
{
protected:
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>> itree;
  void priRBtree()
  {
    print_process("RBtree nodes and instrument colors");
    zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>>::iterator ite1 = itree.begin();
    zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>>::iterator ite2 = itree.end();

    zfwstl::__rb_tree_base_iterator rbtite;
    for (; ite1 != ite2; ++ite1)
    {
      rbtite = zfwstl::__rb_tree_base_iterator(ite1);
      std::cout << *ite1 << "(" << rbtite.node->color << ")";
    }
    std::cout << std::endl;
  }
  void SetUp() override
  {
    itree.insert_unique(23);
    itree.insert_unique(34);
    itree.insert_unique(27);
    itree.insert_unique(15);
    itree.insert_unique(9);
    itree.insert_unique(6);
    itree.insert_unique(8);
    itree.insert_unique(5);
    itree.insert_unique(25);
  }
};
//===============测试用例开始===============
// 测试构造函数
TEST_F(RBTreeTest, Constructor)
{
  priRBtree();
  print_process("Default constructor");
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>> tree1; // 默认构造
  EXPECT_TRUE(tree1.empty());
  EXPECT_EQ(tree1.size(), 0);

  print_process("Copy constructor");
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>> tree2(itree); // 拷贝构造
  EXPECT_EQ(itree, tree2);
  EXPECT_FALSE(itree.empty());

  print_process("Move constructor");
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>> tree3(std::move(tree2)); // 移动构造
  EXPECT_NE(tree3, tree2);
  EXPECT_TRUE(tree2.empty());
}
// 测试operator=复制赋值操作符
TEST_F(RBTreeTest, AssignmentOperator)
{
  print_process("Assignment operator");
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>> tree2 = itree; // 赋值操作运算符
  EXPECT_FALSE(tree2.empty());
  EXPECT_EQ(tree2, itree);
  EXPECT_EQ(tree2.size(), 9);

  print_process("Move Assignment operator"); // 移动赋值操作运算符
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>> tree3 = std::move(tree2);
  EXPECT_TRUE(tree2.empty());
  EXPECT_FALSE(tree3.empty());
  EXPECT_EQ(tree3.size(), 9);
}
// 测试 erase 方法
TEST_F(RBTreeTest, EraseClear)
{
  std::cout << "rbtree's element before erasing:" << std::endl;
  priRBtree();
  print_process("Erase a single element");
  itree.erase(itree.find(25));
  std::cout << "rbtree's element after erasing1:" << std::endl;
  priRBtree();
  EXPECT_EQ(itree.size(), 8);
  EXPECT_EQ(itree.count_unique(2), 0);

  print_process("Erase multiple elements"); // 测试 erase 范围
  auto first = itree.find(5);
  auto last = itree.find(9);
  itree.erase(first, last);
  std::cout << "rbtree's element after erasing2:" << std::endl;
  priRBtree();
  EXPECT_EQ(itree.size(), 5);
  EXPECT_EQ(itree.count_unique(5), 0);
  EXPECT_EQ(itree.count_unique(6), 0);
  EXPECT_EQ(itree.count_unique(8), 0);

  print_process("clear");
  itree.clear();
  EXPECT_TRUE(itree.empty());
  EXPECT_EQ(itree.size(), 0);
}
// 测试 swap 方法
TEST_F(RBTreeTest, Swap)
{
  print_process("Swap");
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>> tree1;
  tree1.insert_unique(23);
  tree1.swap(itree);
  EXPECT_EQ(tree1.size(), 9);
  EXPECT_EQ(itree.size(), 1);
}
TEST_F(RBTreeTest, InsertEraseCount)
{
  print_process("insert_unique: insert a duplicate key"); // 尝试插入重复的键
  itree.insert_unique(25);
  EXPECT_EQ(itree.count_unique(25), 1); // 应该仍然是1，因为键是唯一的

  print_process("insert_equal: insert a duplicate key");
  itree.insert_equal(25);
  itree.insert_equal(25);
  EXPECT_EQ(itree.count_multi(25), 3);

  print_process("erase_unique");
  auto result = itree.erase_unique(25);
  EXPECT_EQ(result, 1); // 返回删除的元素个数
  itree.insert_equal(25);

  print_process("erase_multi");
  result = itree.erase_multi(25);
  EXPECT_EQ(result, 3); // 返回删除的元素个数
}
TEST_F(RBTreeTest, LowerUpperBound)
{
  print_process("lower_bound");
  // 测试存在的键
  auto it = itree.lower_bound(14);
  ASSERT_TRUE(it != itree.end());
  EXPECT_EQ(*it, 15);

  // 测试不存在的键
  it = itree.lower_bound(55);
  EXPECT_TRUE(it == itree.end());

  // 测试小于最小键的值
  it = itree.lower_bound(4);
  EXPECT_TRUE(it != itree.end());
  EXPECT_EQ(*it, 5);

  print_process("upper_bound");
  // 测试不存在的键
  it = itree.upper_bound(3);
  EXPECT_EQ(*it, 5);

  // 测试存在的键
  it = itree.upper_bound(5);
  ASSERT_TRUE(it != itree.end());
  EXPECT_EQ(*it, 6);

  it = itree.upper_bound(9);
  ASSERT_TRUE(it != itree.end());
  EXPECT_EQ(*it, 15);

  print_process("equal_range");
  itree.insert_equal(15);
  itree.insert_equal(15);
  itree.insert_equal(15);
  itree.insert_equal(15);
  // 测试存在的键
  auto range = itree.equal_range(15);
  ASSERT_TRUE(range.first != range.second);
  EXPECT_EQ(zfwstl::distance(range.first, range.second), 5);
  EXPECT_EQ(*(range.first), 15);

  // 测试不存在的键
  range = itree.equal_range(2);
  EXPECT_EQ(range.first, range.second);
}
// 测试一系列反向迭代器rbegin, rend()
TEST_F(RBTreeTest, BeginEndIterators)
{
  print_process("rbegin/end & rcbegin/end");
  auto it = itree.begin();
  EXPECT_EQ(*it, 5); // 检查 begin() 是否指向第一个元素
  auto end_it = itree.end();
  EXPECT_EQ(*(--end_it), 34); // 检查 end() 前一个元素是否是最后一个元素

  auto rbegin_it = itree.rbegin();
  EXPECT_EQ(*rbegin_it, 34); // 检查 rbegin() 是否指向最后一个元素
  auto rend_it = itree.rend();
  EXPECT_EQ(*(--rend_it), 5); // 检查 rend() 前一个元素是否是第一个元素

  auto cbegin_it = itree.cbegin();
  EXPECT_EQ(*cbegin_it, 5); // 检查 cbegin() 是否指向第一个元素
  auto cend_it = itree.cend();
  EXPECT_EQ(*(--cend_it), 34); // 检查 cend() 前一个元素是否是最后一个元素

  auto crbegin_it = itree.crbegin();
  EXPECT_EQ(*crbegin_it, 34); // 检查 crbegin() 是否指向第一个元素
  auto crend_it = itree.crend();
  EXPECT_EQ(*(--crend_it), 5); // 检查 crend() 前一个元素是否是最后一个元素
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_vector_H_