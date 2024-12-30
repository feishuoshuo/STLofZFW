#ifndef ZFWSTL_ALGOBASE_H_
#define ZFWSTL_ALGOBASE_H_
#include <type_traits> // for enable_if()
#include <cstring>     //for memset(), memmove()
#include "../util.h"   //for move()
#include "../iterator.h"
/**
 * 基本算法
 * copy 复制 -if in-place
 * copy_backeard 逆向复制 -if in-place
 * copy_n * 复制n个元素 -if in-place
 * equal 判断两个区间相等与否
 * fill 改填元素值
 * fill 改填元素值, n次
 *
 */
namespace zfwstl
{
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min
  // ===========================max===========================
  // 取二者中的较大值，语义相等时保证返回第一个参数
  template <class T>
  const T &max(const T &lhs, const T &rhs)
  {
    return lhs < rhs ? rhs : lhs;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class T, class Compare>
  const T &max(const T &lhs, const T &rhs, Compare comp)
  {
    return comp(lhs, rhs) ? rhs : lhs;
  }
  // ===========================copy===========================
  // 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
  // 返回result指针指向拷贝后的区间最后一个元素下一个位置
  // input_iterator_tag 版本
  template <class InputIter, class OutputIter>
  OutputIter
  unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
                     zfwstl::input_iterator_tag)
  {
    for (; first != last; ++first, ++result)
    {
      *result = *first;
    }
    return result;
  }

  // ramdom_access_iterator_tag 版本
  template <class RandomIter, class OutputIter>
  OutputIter
  unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result,
                     zfwstl::random_access_iterator_tag)
  {
    for (auto n = last - first; n > 0; --n, ++first, ++result)
    {
      *result = *first;
    }
    return result;
  }

  template <class InputIter, class OutputIter>
  OutputIter
  unchecked_copy(InputIter first, InputIter last, OutputIter result)
  {
    return unchecked_copy_cat(first, last, result, iterator_category(first));
  }

  // 为 trivially_copy_assignable 类型提供特化版本
  template <class Tp, class Up>
  typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
          std::is_trivially_copy_assignable<Up>::value,
      Up *>::type
  unchecked_copy(Tp *first, Tp *last, Up *result)
  {
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
      std::memmove(result, first, n * sizeof(Up));
    return result + n;
  }

  template <class InputIter, class OutputIter>
  OutputIter copy(InputIter first, InputIter last, OutputIter result)
  {
    return unchecked_copy(first, last, result);
  }

  // ===========================copy_backward===============================
  // 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
  // unchecked_copy_backward_cat 的 bidirectional_iterator_tag 版本
  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2
  unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                              BidirectionalIter2 result, zfwstl::bidirectional_iterator_tag)
  {
    while (first != last)
      *--result = *--last;
    return result;
  }

  // unchecked_copy_backward_cat 的 random_access_iterator_tag 版本
  template <class RandomIter1, class BidirectionalIter2>
  BidirectionalIter2
  unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
                              BidirectionalIter2 result, zfwstl::random_access_iterator_tag)
  {
    for (auto n = last - first; n > 0; --n)
      *--result = *--last;
    return result;
  }

  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2
  unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                          BidirectionalIter2 result)
  {
    return unchecked_copy_backward_cat(first, last, result,
                                       iterator_category(first));
  }

  // TAG:为 trivially_copy_assignable 类型提供特化版本
  template <class Tp, class Up>
  typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
          std::is_trivially_copy_assignable<Up>::value,
      Up *>::type
  unchecked_copy_backward(Tp *first, Tp *last, Up *result)
  {
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
    {
      result -= n;
      std::memmove(result, first, n * sizeof(Up));
    }
    return result;
  }

  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2
  copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
  {
    return unchecked_copy_backward(first, last, result);
  }

  // ===========================copy_if===========================
  // 把[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上

  // ===========================copy_n===========================
  // 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
  // 返回一个 pair 分别指向拷贝结束的尾部

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

  // ===========================move_backward===========================
  // 将 [first, last)区间内的元素移动到 [result - (last - first), result)内

  // ===========================equal===========================
  // 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等

  // ===========================fill_n===============================
  // 从 first 位置开始填充 n 个值
  template <class OutputIter, class Size, class T>
  OutputIter unchecked_fill_n(OutputIter first, Size n, const T &value)
  {
    for (; n > 0; --n, ++first)
    {
      *first = value;
    }
    return first;
  }

  // TAG:为 one-byte 类型[占用一个字节（即8位）的变量类型]提供特化版本
  // TAG??:SFINAE（Substitution Failure Is Not An Error）技术的模板元编程技巧
  /**
   * 它用于在模板函数中实现条件编译，即根据类型的特性来决定是否编译该模板的某个实例
   * 只有当 Tp和 Up都是1字节大小的整型，且 Tp不是 bool类型时，编译器才会编译这个模板实例，
   * 并且返回类型是Tp*（指向Tp类型的指针）
   */
  template <class Tp, class Size, class Up>
  typename std::enable_if<
      std::is_integral<Tp>::value && sizeof(Tp) == 1 && // 检查Tp是否是整型，且Tp类型大小是否为1字节
          !std::is_same<Tp, bool>::value &&             // 确保Tp不是bool类型，因为bool虽然也是1字节大小，但它的行为可能与其他1字节整型不同
          std::is_integral<Up>::value && sizeof(Up) == 1,
      Tp *>::type
  unchecked_fill_n(Tp *first, Size n, Up value)
  {
    if (n > 0)
    {
      // TAG: memset函数将 value 填充到从 first 开始的 n 个字节的内存区域
      // 这里将 value 转换为 unsigned char，因为 memset 需要一个字节的值来填充内存
      std::memset(first, (unsigned char)value, (size_t)(n));
    }
    return first + n;
  }

  template <class OutputIter, class Size, class T>
  OutputIter fill_n(OutputIter first, Size n, const T &value)
  {
    return unchecked_fill_n(first, n, value);
  }

  // ===========================fill===========================
  // 为 [first, last)区间内的所有元素填充新值
  template <class ForwardIter, class T>
  void fill_cat(ForwardIter first, ForwardIter last, const T &value,
                zfwstl::forward_iterator_tag)
  {
    for (; first != last; ++first)
      *first = value;
  }

  template <class RandomIter, class T>
  void fill_cat(RandomIter first, RandomIter last, const T &value,
                zfwstl::random_access_iterator_tag)
  {
    auto n = last - first;
    fill_n(first, n, value);
  }

  template <class ForwardIter, class T>
  void fill(ForwardIter first, ForwardIter last, const T &value)
  {
    fill_cat(first, last, value, iterator_category(first));
  }

  // ===========================lexicographical_compare===========================
  /**
   *以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
    (1)如果第一序列的元素较小，返回 true ，否则返回 false
    (2)如果到达 last1 而尚未到达 last2 返回 true
    (3)如果到达 last2 而尚未到达 last1 返回 false
    (4)如果同时到达 last1 和 last2 返回 false
   * */

  // ===========================mismatch===========================
  // 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素

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

#endif // !ZFWSTL_ALGOBASE_H_