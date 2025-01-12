#include "../STL_2/hashtable.h"
#include "../src/functional.h" //for hash<int>
int main()
{
  /**
   * 哈希表hashtable
   */
  zfwstl::hashtable<int,
                    int,
                    zfwstl::hash<int>,
                    zfwstl::identity<int>,
                    zfwstl::equal_to<int>>
      iht(50, zfwstl::hash<int>(), zfwstl::equal_to<int>()); // 指定50个buckets与函数对象
  // cout << iht.size() << endl;
  // cout << iht.bucket_count() << endl;     // 第一个质数
  // cout << iht.max_bucket_count() << endl; // 最后一个质数

  iht.insert_unique(59);
  iht.insert_unique(63);
  iht.insert_unique(108);
  iht.insert_unique(2);
  iht.insert_unique(53);
  iht.insert_unique(55);
  // cout << iht.size() << endl;

  // 声明hashtable迭代器
  // 使用auto可以避免上述长串，注意auto做函数返回类型时不会是引用
  auto ite = iht.begin();

  // 使用迭代器遍历hashtable输出所有节点值
  for (int i = 0; i < iht.size(); ++i, ++ite)
    cout << *ite << ' ';
  std::cout << std::endl;

  // 遍历所有buckets，如果节点个数不为0则打印
  for (int i = 0; i < iht.bucket_count(); ++i)
  {
    int n = iht.elems_in_bucket(i);
    if (n != 0)
      cout << "bucket[" << i << "] has " << n << " elems." << endl;
  }

  // // 验证bucket(list)的容量就是buckets vector的大小
  // for (int i = 0; i <= 47; ++i)
  // {
  //   iht.insert_equal(i);
  // }
  // cout << iht.size() << endl;
  // cout << iht.bucket_count() << endl;

  // // 遍历所有buckets，如果节点个数不为0则打印
  // for (int i = 0; i < iht.bucket_count(); ++i)
  // {
  //   int n = iht.elems_in_bucket(i);
  //   if (n != 0)
  //     cout << "bucket[" << i << "] has " << n << " elems." << endl;
  // }

  // // 使用迭代器遍历hashtable输出所有节点值
  // ite = iht.begin();
  // for (int i = 0; i < iht.size(); ++i, ++ite)
  //   cout << *ite << ' ';
  // cout << endl;

  // cout << *(iht.find(2)) << endl;
  // cout << iht.count(2) << endl;

  // // template argument deduction/substitution failed
  // // iht.insert_unique(string("jjhou"));
  // std::cout << std::endl;
  return 0;
}