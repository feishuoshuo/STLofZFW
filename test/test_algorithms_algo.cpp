#include <iostream>
#include "../STL/vector.h"
#include "../src/algorithms/algo.h"
#include "../src/functional.h" // for equal_to
// 仿函数
template <class T>
struct display
{
  void operator()(const T &x) const
  {
    std::cout << x << ' ';
  }
};

// 仿函数，一元谓词
struct even
{
  bool operator()(int x) const
  {
    return x % 2 ? false : true;
  }
};

// 还是一个仿函数
class even_by_two
{
public:
  int operator()() const
  {
    return _x += 2;
  }

private:
  static int _x;
};

int even_by_two::_x = 0; // 类内声明类外初始化

// int main()
// {
//   int ia[] = {0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8};
//   zfwstl::vector<int> iv(ia, ia + sizeof(ia) / sizeof(int));

//   // 找出相邻元素值相等的第一个元素
//   std::cout << *zfwstl::adjacent_find(iv.begin(), iv.end()) << std::endl;
//   // 找出相邻元素值相等的第一个元素
//   std::cout << *zfwstl::adjacent_find(iv.begin(), iv.end(), equal_to<int>()) << std::endl;

//   // 找出元素值为6的元素个数
//   std::cout << zfwstl::count(iv.begin(), iv.end(), 6) << endl;
//   // 找出元素值小于7的元素个数
//   std::cout << zfwstl::count_if(iv.begin(), iv.end(), bind2nd(less<int>(), 7)) << std::endl;

//   // 找出元素值为4的元素所在位置
//   std::cout << *zfwstl::find(iv.begin(), iv.end(), 4) << endl;
//   // 找出元素值大于2的第一个元素所在位置
//   std::cout << *zfwstl::find_if(iv.begin(), iv.end(), bind2nd(greater<int>(), 2)) << std::endl;

//   // 找出子序列所出现的最后一个位置，加3
//   zfwstl::vector<int> iv2(ia + 6, ia + 8);
//   std::cout << *(zfwstl::find_end(iv.begin(), iv.end(), iv2.begin(), iv2.end()) + 3) << std::endl;

//   // 找出子序列所出现的第一个位置，加3
//   std::cout << *(zfwstl::find_first_of(iv.begin(), iv.end(), iv2.begin(), iv2.end()) + 3) << std::endl;

//   // 迭代iv 每个元素进行display
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;

//   // 以下写法错误，generate的第三个参数是仿函数，本身不得有任何参数
//   // generate(iv.begin(), iv.end(), bind2nd(plus<int>(), 3)); // error

//   // 迭代iv2，对每个元素进行even_by_two
//   zfwstl::generate(iv2.begin(), iv2.end(), even_by_two());
//   zfwstl::for_each(iv2.begin(), iv2.end(), display<int>());
//   std::cout << std::endl;
//   // 迭代指定区间，对每个元素进行even_by_two
//   zfwstl::generate_n(iv2.begin(), 3, even_by_two());
//   zfwstl::for_each(iv2.begin(), iv2.end(), display<int>());
//   std::cout << std::endl;

//   // 删除（但不删除）元素6，尾端可能有残余数据（可以使用erase）
//   zfwstl::remove(iv.begin(), iv.end(), 6);
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;

//   // 删除（但不删除）元素6，结果置于另一区间
//   zfwstl::vector<int> iv3(12);
//   zfwstl::remove_copy(iv.begin(), iv.end(), iv3.begin(), 6);
//   zfwstl::for_each(iv3.begin(), iv3.end(), display<int>());
//   std::cout << std::endl;

//   // 删除（但不删除）元素6，尾端可能有残余数据
//   zfwstl::remove_if(iv.begin(), iv.end(), bind2nd(less<int>(), 6));
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;

//   // 删除（但不删除）小于7的元素，结果置于另一区间
//   zfwstl::remove_copy_if(iv.begin(), iv.end(), iv3.begin(), bind2nd(less<int>(), 7));
//   zfwstl::for_each(iv3.begin(), iv3.end(), display<int>());
//   std::cout << std::endl;

//   // 将所有的元素值6，改为元素值3
//   zfwstl::replace(iv.begin(), iv.end(), 6, 3);
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;
//   // 将所有的元素值3，改为元素值5，结果置于另一区间
//   zfwstl::replace_copy(iv.begin(), iv.end(), iv3.begin(), 3, 5);
//   zfwstl::for_each(iv3.begin(), iv3.end(), display<int>());
//   std::cout << std::endl;

//   // 将所有小于5的元素值，改为元素值2
//   zfwstl::replace_if(iv.begin(), iv.end(), bind2nd(less<int>(), 5), 2);
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;

//   // 将所有等于8的元素值，改为元素值9，结果置于另一区间
//   zfwstl::replace_copy_if(iv.begin(), iv.end(), iv3.begin(), bind2nd(equal_to<int>(), 8), 9);
//   zfwstl::for_each(iv3.begin(), iv3.end(), display<int>());
//   std::cout << std::endl;

//   // 逆向重排每一个元素
//   zfwstl::reverse(iv.begin(), iv.end());
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;
//   // 逆向重排每一个元素，结果置于另一区间
//   zfwstl::reverse_copy(iv.begin(), iv.end(), iv3.begin());
//   zfwstl::for_each(iv3.begin(), iv3.end(), display<int>());
//   std::cout << std::endl;

//   // 旋转（互换元素）[first, middle]和[middle, last]
//   zfwstl::rotate(iv.begin(), iv.begin() + 4, iv.end());
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;
//   // 旋转（互换元素）[first, middle]和[middle, last]
//   // 结果置于另一区间
//   zfwstl::rotate_copy(iv.begin(), iv.begin() + 5, iv.end(), iv3.begin());
//   zfwstl::for_each(iv3.begin(), iv3.end(), display<int>());
//   std::cout << std::endl;

//   // 查找某个子序列的第一次出现地点
//   int ia2[3] = {2, 8};
//   zfwstl::vector<int> iv4(ia2, ia2 + 2);
//   std::cout << *zfwstl::search(iv.begin(), iv.end(), iv4.begin(), iv4.end()) << std::endl;
//   // 查找连续出现2个8的子序列起点
//   std::cout << *zfwstl::search_n(iv.begin(), iv.end(), 2, 8) << endl;
//   // 查找连续出现3个小于8的子序列起点
//   std::cout << *zfwstl::search_n(iv.begin(), iv.end(), 3, 8, less<int>()) << std::endl;

//   // 将区间元素互换，第二区间个数不应小于第一区间个数
//   zfwstl::swap_ranges(iv4.begin(), iv4.end(), iv.begin());
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;
//   zfwstl::for_each(iv4.begin(), iv4.end(), display<int>());
//   std::cout << std::endl;

//   // 改变区间的值，全部减2，原地搬运
//   zfwstl::transform(iv.begin(), iv.end(), iv.begin(), bind2nd(minus<int>(), 2));
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;

//   // 改变区间的值，令 第二区间的元素值加到第一区间上
//   zfwstl::transform(iv.begin(), iv.end(), iv.begin(), iv.begin(), plus<int>());
//   zfwstl::for_each(iv.begin(), iv.end(), display<int>());
//   std::cout << std::endl;

//   // *******************************************

//   zfwstl::vector<int> iv5(ia, ia + sizeof(ia) / sizeof(int));
//   zfwstl::vector<int> iv6(ia + 4, ia + 8);
//   zfwstl::vector<int> iv7(15);
//   zfwstl::for_each(iv5.begin(), iv5.end(), display<int>());
//   std::cout << std::endl;
//   zfwstl::for_each(iv6.begin(), iv6.end(), display<int>());
//   std::cout << std::endl;

//   std::cout << *zfwstl::max_element(iv5.begin(), iv5.end()) << std::endl;
//   std::cout << *zfwstl::min_element(iv5.begin(), iv5.end()) << std::endl;

//   // 判断是否iv6内的所有元素都出现于iv5中
//   std::cout << zfwstl::includes(iv5.begin(), iv5.end(), iv6.begin(), iv6.end()) << std::endl;

//   // 将两个序列合并，必须有序
//   zfwstl::merge(iv5.begin(), iv5.end(), iv6.begin(), iv6.end(), iv7.begin());
//   zfwstl::for_each(iv7.begin(), iv7.end(), display<int>());
//   std::cout << std::endl;

//   // 符合条件的元素放在容器前端，其他放后端
//   zfwstl::partition(iv7.begin(), iv7.end(), even());
//   zfwstl::for_each(iv7.begin(), iv7.end(), display<int>());
//   std::cout << std::endl;

//   // 去除连续重复元素
//   zfwstl::unique(iv5.begin(), iv5.end());
//   zfwstl::for_each(iv5.begin(), iv5.end(), display<int>());
//   std::cout << std::endl;
//   // 去除连续重复元素，结果置于另一区间
//   zfwstl::unique_copy(iv5.begin(), iv5.end(), iv7.begin());
//   zfwstl::for_each(iv7.begin(), iv7.end(), display<int>());
//   std::cout << std::endl;
// }

int main()
{
  int ia[] = {12, 17, 20, 22, 23, 30, 33, 40};
  zfwstl::vector<int> iv(ia, ia + sizeof(ia) / sizeof(int));

  std::cout << *zfwstl::lower_bound(iv.begin(), iv.end(), 21) << std::endl; // 22
  std::cout << *zfwstl::upper_bound(iv.begin(), iv.end(), 21) << std::endl; // 22
  std::cout << *zfwstl::lower_bound(iv.begin(), iv.end(), 22) << std::endl; // 22
  std::cout << *zfwstl::upper_bound(iv.begin(), iv.end(), 22) << std::endl; // 23

  // 有序区间可以二分查找
  std::cout << zfwstl::binary_search(iv.begin(), iv.end(), 33) << std::endl;
  std::cout << zfwstl::binary_search(iv.begin(), iv.end(), 34) << std::endl;

  // 下一个排列组合
  zfwstl::next_permutation(iv.begin(), iv.end());
  zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  // 上一个排列组合
  zfwstl::prev_permutation(iv.begin(), iv.end());
  zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  // 随机重排
  zfwstl::random_shuffle(iv.begin(), iv.end());
  zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  // 部分排序
  zfwstl::partial_sort(iv.begin(), iv.begin() + 4, iv.end());
  zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  // 排序，默认递增
  zfwstl::sort(iv.begin(), iv.end());
  zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  // 排序，指定递减
  zfwstl::sort(iv.begin(), iv.end(), greater<int>());
  zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  iv.push_back(22);
  iv.push_back(30);
  iv.push_back(17);

  // // 排序，保持原相对位置
  // zfwstl::stable_sort(iv.begin(), iv.end());
  // zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  // std::cout << std::endl;

  // 对有序区间，找出一个区间，每个元素都等于特定元素值，返回头尾迭代器
  // 找不到则返回可插入特定元素的头尾迭代器
  zfwstl::pair<zfwstl::vector<int>::iterator, zfwstl::vector<int>::iterator> pairIte;
  pairIte = zfwstl::equal_range(iv.begin(), iv.end(), 22);
  std::cout << *(pairIte.first) << std::endl;  // 22 lower_bound
  std::cout << *(pairIte.second) << std::endl; // 23 upper_bound

  pairIte = zfwstl::equal_range(iv.begin(), iv.end(), 25);
  std::cout << *(pairIte.first) << std::endl;  // 30 lower_bound
  std::cout << *(pairIte.second) << std::endl; // 30 upper_bound

  zfwstl::random_shuffle(iv.begin(), iv.end());
  zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  // 将小于40的元素放左边，其他放右边，不稳定算法
  zfwstl::nth_element(iv.begin(), iv.begin() + 5, iv.end());
  zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  // // 符合条件放左边，否则放右边，稳定算法
  // zfwstl::stable_partition(iv.begin(), iv.end(), even());
  // zfwstl::copy(iv.begin(), iv.end(), zfwstl::ostream_iterator<int>(std::cout, " "));
  // std::cout << std::endl;
}