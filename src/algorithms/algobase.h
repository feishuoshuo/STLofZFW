#ifndef ZFWSTL_ALGOBASE_H_
#define ZFWSTL_ALGOBASE_H_
#include <cstring>       //for memset(), memmove()
#include "../util.h"     //for move(), swap(), pair
#include "../iterator.h" //for value_type(), iterator_traits
/**
 * 基本算法
 * copy 复制 -if in-place
 * copy_backeard 逆向复制 -if in-place
 * copy_n * 复制n个元素 -if in-place
 * zfw==copy_if
 * equal 判断两个区间相等与否
 * fill 改填元素值
 * fill_n 改填元素值, n次
 * iter_swap 元素互换
 * max/min
 * mismatch 找出不匹配点
 * swap 交换(对调)
 */
namespace zfwstl
{
  // 用来处理宏定义冲突的
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min
  // ===========================copy===========================
  // 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
  // 返回result指针指向拷贝后的区间最后一个元素下一个位置
  /**
   * copy算法需要注意一个情况：
   * 如果copy区间有重叠==>std::memmove()会先将整个输入区间的内容复制下来，没有被覆盖的危险
   *
   * copy更改的是指定区间内的迭代器所指对象，而非迭代器本身，也就是不能直接用它将元素插入空容器中！！
   * 如果想将元素插入(而非赋值)序列之内：
   *    1- 使用序列容器的insert成员函数
   *    2- 使用copy算法搭配insert_iterator
   */
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
  /**
   * 倒施逆行
   */
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
  // ===========================copy_n===========================
  // 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
  // 返回一个 pair 分别指向拷贝结束的尾部

  // ===========================copy_if===========================
  // 把[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上

  // ===========================equal===========================
  // 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
  template <class InputIter1, class InputIter2>
  bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
  {
    // 若序列1的元素个数多于序列2，则会造成不可预测的结果
    for (; first1 != last1; ++first1, ++first2)
    {
      if (*first1 != *first2)
        return false;
    }
    return true;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter1, class InputIter2, class Compared>
  bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compared comp)
  {
    for (; first1 != last1; ++first1, ++first2)
    {
      if (!comp(*first1, *first2))
        return false;
    }
    return true;
  }

  // TAG: zfw优化后的equal
  template <class InputIter1, class InputIter2>
  bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2)
  {
    // 检查两个序列的长度是否相等
    if (zfwstl::distance(first1, last1) != zfwstl::distance(first2, last2))
      return false;

    // 比较两个序列的元素
    for (; first1 != last1; ++first1, ++first2)
    {
      if (*first1 != *first2)
        return false;
    }
    return true;
  }

  // ===========================fill_n===============================
  // 从 first 位置开始填充 n 个值
  /**
   * 实际fill_n是一种覆写操作(overwrite), 所以一旦操作区间超越了容器大小，就会造成不可预期的结果
   * 解决办法之一：利用inserter()[iterator adapter]产生一个有能力插入而非覆写的迭代器
   * 如：fill_n(inserter(iv, iv.begin(), 5, 7));
   */
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
    zfwstl::fill_n(first, n, value);
  }

  template <class ForwardIter, class T>
  void fill(ForwardIter first, ForwardIter last, const T &value)
  {
    fill_cat(first, last, value, iterator_category(first));
  }

  // ===========================iter_swap===========================
  // 将两个迭代器所指对象对调
  // TinySTL的写法
  // template <class FIter1, class FIter2>
  // void iter_swap(FIter1 lhs, FIter2 rhs)
  // {
  //   zfwstl::swap(*lhs, *rhs);
  // }
  template <class FIter1, class FIter2>
  inline void iter_swap(FIter1 lhs, FIter2 rhs)
  {
    typename zfwstl::iterator_traits<FIter1>::value_type tmp = *lhs;
    *lhs = *rhs;
    *rhs = tmp;
  }

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

  // ===========================min ===========================
  // 取二者中的较小值，语义相等时保证返回第一个参数
  template <class T>
  const T &min(const T &lhs, const T &rhs)
  {
    return rhs < lhs ? rhs : lhs;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class T, class Compare>
  const T &min(const T &lhs, const T &rhs, Compare comp)
  {
    return comp(rhs, lhs) ? rhs : lhs;
  }

  // ===========================mismatch===========================
  // 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
  template <class InputIter1, class InputIter2>
  zfwstl::pair<InputIter1, InputIter2>
  mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
  {
    // 显然，序列1的元素个数必须多过序列2的元素个数，否则结果无可预期
    while (first1 != last1 && *first1 == *first2)
    {
      ++first1;
      ++first2;
    }
    return zfwstl::pair<InputIter1, InputIter2>(first1, first2);
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter1, class InputIter2, class Compred>
  zfwstl::pair<InputIter1, InputIter2>
  mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compred comp)
  {
    while (first1 != last1 && comp(*first1, *first2))
    {
      ++first1;
      ++first2;
    }
    return zfwstl::pair<InputIter1, InputIter2>(first1, first2);
  }
  // TAG: zfw优化后的mismatch
  template <class InputIter1, class InputIter2>
  zfwstl::pair<InputIter1, InputIter2>
  mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2)
  {
    // 比较两个序列的元素，直到任一序列结束或找到失配的元素
    while (first1 != last1 && first2 != last2 && *first1 == *first2)
    {
      ++first1;
      ++first2;
    }
    // 如果两个序列都结束了，返回末尾迭代器对
    if (first1 == last1 && first2 == last2)
    {
      return zfwstl::pair<InputIter1, InputIter2>(last1, last2);
    }
    // 否则返回第一对失配的元素迭代器
    return zfwstl::pair<InputIter1, InputIter2>(first1, first2);
  }

  // // ===========================swap===========================
  // template <class T>
  // inline void swap(T &a, T &b)
  // {
  //   T tmp = a;
  //   a = b;
  //   b = tmp;
  // }
}

#endif // !ZFWSTL_ALGOBASE_H_