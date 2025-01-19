#ifndef GOOGLETEST_SAMPLES_vector_H_
#define GOOGLETEST_SAMPLES_vector_H_
#include "../../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include <cstddef> // for size_t, ptrdiff_t
#include <string>
#include "../STL_2/hashtable.h"
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
  std::cout << "[----------------- Run container test : hashtable -----------------]\n";
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
class HashtableTest : public ::testing::Test
{
protected:
  zfwstl::hashtable<int,
                    int,
                    zfwstl::hash<int>,
                    zfwstl::identity<int>,
                    zfwstl::equal_to<int>>
      iht;
  HashtableTest() : iht(50, zfwstl::hash<int>(), zfwstl::equal_to<int>()) {}
  void priRHashtable()
  {
    print_process("hashtable:");
    // 遍历所有buckets，如果节点个数不为0则打印
    for (int i = 0; i < iht.bucket_count(); ++i)
    {
      int n = iht.elems_in_bucket(i);
      if (n != 0)
      {
        cout << "bucket[" << i << "] has " << n << " elems:" << endl;
        auto bucket = iht.get_buckets()[i];
        while (bucket)
        {
          auto key = iht.get_key_function()(bucket->val); // 使用公共访问器函数
          auto count = iht.count(key);
          auto bucket_index = iht.bucket_index_of(key); // 获取键值所在的桶的索引
          cout << "  key: " << key << ", value: " << bucket->val << ", count: " << count << ", bucket index: " << bucket_index << endl;
          bucket = bucket->next;
        }
      }
    }
    std::cout << std::endl;
  }
  void SetUp() override
  {
    iht.insert_unique(23);
    iht.insert_unique(34);
    iht.insert_unique(27);
    iht.insert_unique(15);
    iht.insert_unique(9);
    iht.insert_unique(6);
    iht.insert_unique(8);
    iht.insert_unique(5);
    iht.insert_unique(25);
  }
};
//===============测试用例开始===============
// 测试构造函数
TEST_F(HashtableTest, Constructor)
{
  priRHashtable();
  print_process("common constructor");
  zfwstl::hashtable<int,
                    int,
                    zfwstl::hash<int>,
                    zfwstl::identity<int>,
                    zfwstl::equal_to<int>>
      iht1(50, zfwstl::hash<int>(), zfwstl::equal_to<int>());
  EXPECT_TRUE(iht1.empty());
  EXPECT_EQ(iht1.size(), 0);

  print_process("Copy constructor");
  zfwstl::hashtable<int,
                    int,
                    zfwstl::hash<int>,
                    zfwstl::identity<int>,
                    zfwstl::equal_to<int>>
      iht2(iht); // 拷贝构造
  EXPECT_FALSE(iht.empty());
  EXPECT_FALSE(iht2.empty());

  print_process("Move constructor");
  zfwstl::hashtable<int,
                    int,
                    zfwstl::hash<int>,
                    zfwstl::identity<int>,
                    zfwstl::equal_to<int>>
      iht3(zfwstl::move(iht2)); // 移动构造
  EXPECT_TRUE(iht2.empty());
}
// 测试operator=复制赋值操作符
TEST_F(HashtableTest, AssignmentOperator)
{
  print_process("Assignment operator");
  zfwstl::hashtable<int,
                    int,
                    zfwstl::hash<int>,
                    zfwstl::identity<int>,
                    zfwstl::equal_to<int>>
      iht3 = iht; // 赋值操作运算符
  EXPECT_FALSE(iht3.empty());
  EXPECT_EQ(iht3.size(), 9);

  print_process("Move Assignment operator"); // 移动赋值操作运算符
  zfwstl::hashtable<int,
                    int,
                    zfwstl::hash<int>,
                    zfwstl::identity<int>,
                    zfwstl::equal_to<int>>
      iht4 = zfwstl::move(iht3);
  EXPECT_TRUE(iht3.empty());
  EXPECT_FALSE(iht4.empty());
  EXPECT_EQ(iht4.size(), 9);
}
// 测试 erase 方法
TEST_F(HashtableTest, EraseClear)
{
  std::cout << "rbtree's element before erasing:" << std::endl;
  priRHashtable();
  print_process("Erase a single element");
  iht.erase(iht.find(25));
  std::cout << "rbtree's element after erasing1:" << std::endl;
  priRHashtable();
  EXPECT_EQ(iht.size(), 8);
  EXPECT_EQ(iht.count(2), 0);

  print_process("Erase multiple elements"); // 测试 erase 范围
  auto first = iht.find(5);
  auto last = iht.find(9);
  iht.erase(first, last);
  std::cout << "rbtree's element after erasing2:" << std::endl;
  priRHashtable();
  EXPECT_EQ(iht.size(), 5);
  EXPECT_EQ(iht.count(5), 0);
  EXPECT_EQ(iht.count(6), 0);
  EXPECT_EQ(iht.count(8), 0);

  print_process("clear");
  iht.clear();
  EXPECT_TRUE(iht.empty());
  EXPECT_EQ(iht.size(), 0);
}
// 测试 swap 方法
TEST_F(HashtableTest, Swap)
{
  print_process("Swap");
  zfwstl::hashtable<int,
                    int,
                    zfwstl::hash<int>,
                    zfwstl::identity<int>,
                    zfwstl::equal_to<int>>
      iht1(50, zfwstl::hash<int>(), zfwstl::equal_to<int>());
  iht1.insert_unique(23);
  iht1.swap(iht);
  EXPECT_EQ(iht1.size(), 9);
  EXPECT_EQ(iht.size(), 1);
}
TEST_F(HashtableTest, InsertEraseCount)
{
  print_process("insert_unique: insert a duplicate key"); // 尝试插入重复的键
  iht.insert_unique(25);
  EXPECT_EQ(iht.count(25), 1); // 应该仍然是1，因为键是唯一的

  print_process("insert_equal: insert a duplicate key");
  iht.insert_equal(25);
  iht.insert_equal(25);
  EXPECT_EQ(iht.count(25), 3);

  print_process("erase_unique");
  auto result = iht.erase_unique(25);
  EXPECT_EQ(result, 1); // 返回删除的元素个数
  iht.insert_equal(25);

  priRHashtable();
  print_process("erase_multi");
  result = iht.erase_multi(25);
  EXPECT_EQ(result, 3); // 返回删除的元素个数
}
// 测试一系列反向迭代器rbegin, rend()
TEST_F(HashtableTest, BeginEndIterators)
{
  print_process("rbegin/end");
  auto it = iht.begin();
  EXPECT_EQ(*it, 5); // 检查 begin() 是否指向第一个元素

  auto cbegin_it = iht.cbegin();
  EXPECT_EQ(*cbegin_it, 5); // 检查 cbegin() 是否指向第一个元素
}
int main(int argc, char **argv)
{
  print_start();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif // GOOGLETEST_SAMPLES_vector_H_