#ifndef ZFWSTL_SET_ALGO_H_
#define ZFWSTL_SET_ALGO_H_
#include "algobase.h" //for copy()
/**
 * set的四种算法: [前提：set是一种sorted range]
 * union并集
 * intersection交集
 * difference差集
 * symmetric_difference对称差集
 */

namespace zfwstl
{
  // ===========================set_union===========================
  // 属于稳定操作, 即输入区间内的每个元素的相对顺序都不会改变
  // 计算 S1∪S2 的结果并保存到 result 中，返回一个迭代器指向输出结果的尾部
  template <class InputIter1, class InputIter2, class OutputIter>
  OutputIter set_union(InputIter1 first1, InputIter1 last1,
                       InputIter2 first2, InputIter2 last2,
                       OutputIter result)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (*first1 < *first2)
      {
        *result = *first1;
        ++first1;
      }
      else if (*first2 < *first1)
      {
        *result = *first2;
        ++first2;
      }
      else
      {
        *result = *first1;
        ++first1;
        ++first2;
      }
      ++result;
    }
    // 将剩余元素拷贝到 result
    // 此可区间1与区间2之中有一个空白区间
    // TAG: 写法注意！
    return zfwstl::copy(first2, last2, zfwstl::copy(first1, last1, result));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter1, class InputIter2, class OutputIter, class Compared>
  OutputIter set_union(InputIter1 first1, InputIter1 last1,
                       InputIter2 first2, InputIter2 last2,
                       OutputIter result, Compared comp)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (comp(*first1, *first2))
      {
        *result = *first1;
        ++first1;
      }
      else if (comp(*first2, *first1))
      {
        *result = *first2;
        ++first2;
      }
      else
      {
        *result = *first1;
        ++first1;
        ++first2;
      }
      ++result;
    }
    // 将剩余元素拷贝到 result
    return zfwstl::copy(first2, last2, zfwstl::copy(first1, last1, result));
  }

  // ===========================set_intersection===========================
  // 也属于稳定操作
  // 计算 S1∩S2 的结果并保存到 result 中，返回一个迭代器指向输出结果的尾部
  template <class InputIter1, class InputIter2, class OutputIter>
  OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                              InputIter2 first2, InputIter2 last2,
                              OutputIter result)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (*first1 < *first2)
      {
        ++first1;
      }
      else if (*first2 < *first1)
      {
        ++first2;
      }
      else
      {
        *result = *first1;
        ++first1;
        ++first2;
        ++result;
      }
    }
    return result;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter1, class InputIter2, class OutputIter, class Compared>
  OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                              InputIter2 first2, InputIter2 last2,
                              OutputIter result, Compared comp)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (comp(*first1, *first2))
      {
        ++first1;
      }
      else if (comp(*first2, *first1))
      {
        ++first2;
      }
      else
      {
        *result = *first1;
        ++first1;
        ++first2;
        ++result;
      }
    }
    return result;
  }

  // ===========================set_difference===========================
  // 计算 S1-S2 的结果并保存到 result 中，返回一个迭代器指向输出结果的尾部
  // 也属于稳定操作
  // "出现于S1但不出现于S2"
  template <class InputIter1, class InputIter2, class OutputIter>
  OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2,
                            OutputIter result)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (*first1 < *first2)
      {
        *result = *first1;
        ++first1;
        ++result;
      }
      else if (*first2 < *first1)
      {
        ++first2;
      }
      else
      {
        ++first1;
        ++first2;
      }
    }
    return zfwstl::copy(first1, last1, result);
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter1, class InputIter2, class OutputIter, class Compared>
  OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2,
                            OutputIter result, Compared comp)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (comp(*first1, *first2))
      {
        *result = *first1;
        ++first1;
        ++result;
      }
      else if (comp(*first2, *first1))
      {
        ++first2;
      }
      else
      {
        ++first1;
        ++first2;
      }
    }
    return zfwstl::copy(first1, last1, result);
  }

  // ===========================set_symmetric_difference===========================
  // 计算 (S1-S2)∪(S2-S1) 的结果并保存到 result 中，返回一个迭代器指向输出结果的尾部
  // 也属于稳定操作
  //"出现于S2但不出现于S1"以及"出现于S1但不出现于S2"
  template <class InputIter1, class InputIter2, class OutputIter>
  OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                      InputIter2 first2, InputIter2 last2,
                                      OutputIter result)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (*first1 < *first2)
      {
        *result = *first1;
        ++first1;
        ++result;
      }
      else if (*first2 < *first1)
      {
        *result = *first2;
        ++first2;
        ++result;
      }
      else
      {
        ++first1;
        ++first2;
      }
    }
    return zfwstl::copy(first2, last2, zfwstl::copy(first1, last1, result));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter1, class InputIter2, class OutputIter, class Compared>
  OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                      InputIter2 first2, InputIter2 last2,
                                      OutputIter result, Compared comp)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (comp(*first1, *first2))
      {
        *result = *first1;
        ++first1;
        ++result;
      }
      else if (comp(*first2, *first1))
      {
        *result = *first2;
        ++first2;
        ++result;
      }
      else
      {
        ++first1;
        ++first2;
      }
    }
    return zfwstl::copy(first2, last2, zfwstl::copy(first1, last1, result));
  }
}

#endif // !ZFWSTL_SET_ALGO_H_
