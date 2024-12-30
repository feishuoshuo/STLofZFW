#ifndef ZFWSTL_ALGO_H_
#define ZFWSTL_ALGO_H_
/**
 *
 * adjacent_find 查找相邻而重复(或符合某条件)的元素
 * binary_search 二分查找
 * count 计数
 * count_if 在特定条件下计数
 * equal_range 试图在有序区间中寻找某值(返回一个上下限区间)
 * find 循序查找
 * find_if 循序查找符合特定条件者
 * find_end 查找某个子序列的最后一次出现点
 * find_first 查找某些元素首次出现点
 * for_each 对区间内的每一个元素实施某操作
 * generate
 * generate_n
 *
 *
 *
 * inplace_merge 合并并就地替换(覆盖上去)
 */
namespace zfwstl
{
  // ===========================find===========================
  // 在[first, last)区间内找到等于 value 的元素，返回指向该元素的迭代器
  template <class InputIter, class T>
  InputIter
  find(InputIter first, InputIter last, const T &value)
  {
    while (first != last && *first != value)
      ++first;
    return first;
  }

}

#endif // !ZFWSTL_ALGO_H_
