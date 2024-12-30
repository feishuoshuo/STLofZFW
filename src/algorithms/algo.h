#ifndef ZFWSTL_ALGO_H_
#define ZFWSTL_ALGO_H_
#include <type_traits> // for enable_if()
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
 * find_first_of 查找某些元素首次出现点
 * for_each 对区间内的每一个元素实施某操作
 * generate 以特定操作之运算结果填充特定区间内的元素
 * generate_n 以特定操作之运算结果填充 n 个元素内容
 * includes 是否涵盖于某序列之中
 * inplace_merge 合并并就地替换(覆盖上去)
 * is_heap * 判断某区间是否为一个heap
 * is_sorted * 判断某区间是否已排序
 * lower_bound 将特定元素插入区间之内而不影响区间之原本排序
 * max_element 最大值所在位置
 * merge 合并两个序列
 * min_element 最小值所在位置
 * next_permutation
 * nth_elemnt
 * partial_sort
 * partial_sort_copy
 * partition
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

  // ===========================move===========================
  // 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
  // input_iterator_tag 版本
  template <class InputIter, class OutputIter>
  OutputIter
  unchecked_move_cat(InputIter first, InputIter last, OutputIter result,
                     zfwstl::input_iterator_tag)
  {
    for (; first != last; ++first, ++result)
    {
      *result = zfwstl::move(*first);
    }
    return result;
  }

  // ramdom_access_iterator_tag 版本
  template <class RandomIter, class OutputIter>
  OutputIter
  unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result,
                     zfwstl::random_access_iterator_tag)
  {
    for (auto n = last - first; n > 0; --n, ++first, ++result)
    {
      *result = zfwstl::move(*first);
    }
    return result;
  }

  template <class InputIter, class OutputIter>
  OutputIter
  unchecked_move(InputIter first, InputIter last, OutputIter result)
  {
    return unchecked_move_cat(first, last, result, iterator_category(first));
  }

  // 为 trivially_copy_assignable 类型提供特化版本
  template <class Tp, class Up>
  typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
          std::is_trivially_move_assignable<Up>::value,
      Up *>::type
  unchecked_move(Tp *first, Tp *last, Up *result)
  {
    const size_t n = static_cast<size_t>(last - first);
    if (n != 0)
      std::memmove(result, first, n * sizeof(Up));
    return result + n;
  }

  template <class InputIter, class OutputIter>
  OutputIter move(InputIter first, InputIter last, OutputIter result)
  {
    return unchecked_move(first, last, result);
  }
  // ===========================for_each===========================
  // 使用一个函数对象 f 对[first, last)区间内的每个元素执行一个 operator() 操作，但不能改变元素内容
  // f() 可返回一个值，但该值会被忽略
  template <class InputIter, class Function>
  Function for_each(InputIter first, InputIter last, Function f)
  {
    for (; first != last; ++first)
    {
      f(*first);
    }
    return f;
  }
  // ===========================move_backward===========================
  // 将 [first, last)区间内的元素移动到 [result - (last - first), result)内

  //=========================lower_bound=================================
  /**
   * 使用lower_bound()，序列需先排序
   * 在[first, last)中查找第一个不小于 value 的元素，并返回指向它的迭代器，若没有则返回 last
   */
  // lbound_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter, class T>
  ForwardIter
  lbound_dispatch(ForwardIter first, ForwardIter last,
                  const T &value, forward_iterator_tag)
  {
    auto len = zfwstl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first;
      zfwstl::advance(middle, half);
      if (*middle < value)
      {
        first = middle;
        ++first;
        len = len - half - 1;
      }
      else
      {
        len = half;
      }
    }
    return first;
  }

  // lbound_dispatch 的 random_access_iterator_tag 版本
  template <class RandomIter, class T>
  RandomIter
  lbound_dispatch(RandomIter first, RandomIter last,
                  const T &value, random_access_iterator_tag)
  {
    auto len = last - first;
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first + half;
      if (*middle < value)
      {
        first = middle + 1;
        len = len - half - 1;
      }
      else
      {
        len = half;
      }
    }
    return first;
  }

  template <class ForwardIter, class T>
  ForwardIter
  lower_bound(ForwardIter first, ForwardIter last, const T &value)
  {
    return zfwstl::lbound_dispatch(first, last, value, iterator_category(first));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  // lbound_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter, class T, class Compared>
  ForwardIter
  lbound_dispatch(ForwardIter first, ForwardIter last,
                  const T &value, forward_iterator_tag, Compared comp)
  {
    auto len = zfwstl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first;
      zfwstl::advance(middle, half);
      if (comp(*middle, value))
      {
        first = middle;
        ++first;
        len = len - half - 1;
      }
      else
      {
        len = half;
      }
    }
    return first;
  }

  // lbound_dispatch 的 random_access_iterator_tag 版本
  template <class RandomIter, class T, class Compared>
  RandomIter
  lbound_dispatch(RandomIter first, RandomIter last,
                  const T &value, random_access_iterator_tag, Compared comp)
  {
    auto len = last - first;
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first + half;
      if (comp(*middle, value))
      {
        first = middle + 1;
        len = len - half - 1;
      }
      else
      {
        len = half;
      }
    }
    return first;
  }

  template <class ForwardIter, class T, class Compared>
  ForwardIter
  lower_bound(ForwardIter first, ForwardIter last, const T &value, Compared comp)
  {
    return zfwstl::lbound_dispatch(first, last, value, iterator_category(first), comp);
  }

}

#endif // !ZFWSTL_ALGO_H_
