#ifndef ZFWSTL_NUMERIC_H_
#define ZFWSTL_NUMERIC_H_
#include "../functional.h" //for multiplies, identity_element
#include <string>
/**
 * 数值算法
 * accumulate 元素累计
 * adjacent_dfference 相邻元素的差额
 * inner_product 内积
 * Iota * 在某区间填入某指定值的递增序列
 * lexicographical_compare 以字典顺序进行比较
 * partial_sum 局部求和 -if in-place
 * power * 幂次方，表达式可指定
 */
namespace zfwstl
{ // ===========================accumulate===========================
  // 版本1：以初值 init 对每个元素进行累加
  template <class InputIter, class T>
  T accumulate(InputIter first, InputIter last, T init)
  {
    for (; first != last; ++first)
    {
      init += *first;
    }
    return init;
  }

  // 版本2：以初值 init 对每个元素进行二元操作, 二元操作符不必满足交换律和结合律
  template <class InputIter, class T, class BinaryOp>
  T accumulate(InputIter first, InputIter last, T init, BinaryOp binary_op)
  {
    for (; first != last; ++first)
    {
      init = binary_op(init, *first);
    }
    return init;
  }

  // ===========================adjacent_difference===========================
  // 版本1：计算相邻元素的差值，结果保存到以 result 为起始的区间上
  template <class InputIter, class OutputIter>
  OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result)
  {
    if (first == last)
      return result;
    *result = *first; // 记录第一个元素
    auto value = *first;
    while (++first != last)
    {
      auto tmp = *first;
      *++result = tmp - value;
      value = tmp;
    }
    return ++result;
  }

  // 版本2：自定义相邻元素的二元操作
  template <class InputIter, class OutputIter, class BinaryOp>
  OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result,
                                 BinaryOp binary_op)
  {
    if (first == last)
      return result;
    *result = *first; // 记录第一个元素
    auto value = *first;
    while (++first != last)
    {
      auto tmp = *first;
      *++result = binary_op(tmp, value); // 将相邻两元素的运算结果，赋值给目的端
      value = tmp;
    }
    return ++result;
  }

  // ===========================inner_product===========================
  // 版本1：以 init 为初值，计算两个区间的内积
  template <class InputIter1, class InputIter2, class T>
  T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init)
  {
    for (; first1 != last1; ++first1, ++first2)
    {
      init = init + (*first1 * *first2);
    }
    return init;
  }

  // 版本2：自定义 operator+ 和 operator*, , 二元操作符不必满足交换律和结合律
  template <class InputIter1, class InputIter2, class T, class BinaryOp1, class BinaryOp2>
  T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init,
                  BinaryOp1 binary_op1, BinaryOp2 binary_op2)
  {
    for (; first1 != last1; ++first1, ++first2)
    {
      // 以外界提供的仿函数来取代第一版本中operator*和opertator+
      init = binary_op1(init, binary_op2(*first1, *first2));
    }
    return init;
  }

  // ===========================iota===========================
  // SGI专属算法，并不在STL标准之列
  // 填充[first, last)，以 value 为初值开始递增
  // [first, last)区间内填入value, value+1, value+2...
  template <class ForwardIter, class T>
  void iota(ForwardIter first, ForwardIter last, T value)
  {
    while (first != last)
    {
      *first++ = value;
      ++value;
    }
  }
  // ===========================lexicographical_compare===========================
  // 比较两个序列（例如两个字符串）是否按字典顺序（lexicographical order）排序的函数
  // TODO:lexicographical_compare
  bool lexicographical_compare(const std::string &str1, const std::string &str2)
  {
    return std::lexicographical_compare(str1.begin(), str1.end(), str2.begin(), str2.end());
  }

  // ===========================partial_sum===========================
  // 返回：输出区间的最尾端位置：result + (last - first)
  // 版本1：计算局部累计求和，结果保存到以 result 为起始的区间上
  template <class InputIter, class OutputIter>
  OutputIter partial_sum(InputIter first, InputIter last, OutputIter result)
  {
    if (first == last)
      return result;
    *result = *first; // 记录第一个元素
    auto value = *first;
    while (++first != last)
    {
      value = value + *first; // 前n个元素总和
      *++result = value;      // 指定给目的端
    }
    return ++result;
  }

  // 版本2：进行局部进行自定义二元操作
  template <class InputIter, class OutputIter, class BinaryOp>
  OutputIter partial_sum(InputIter first, InputIter last, OutputIter result,
                         BinaryOp binary_op)
  {
    if (first == last)
      return result;
    *result = *first; // 记录第一个元素
    auto value = *first;
    while (++first != last)
    {
      value = binary_op(value, *first);
      *++result = value;
    }
    return ++result;
  }

  // ===========================power===========================
  // SGI专属算法，并不在STL标准之列
  // 版本1：乘幂
  template <class T, class Integer>
  inline T power(T x, Integer n) { return power(x, n, zfwstl::multiplies<T>()); } // 指定运算形式为乘法

  // 版本2：幂次方。如果指定为乘法运算，则当 n>=0时，返回x^n
  /**
   * 使用了快速幂算法，这是一种高效计算幂的方法，特别适用于大指数的情况。
   * 快速幂算法通过将指数分解为二进制形式，并利用指数的二进制表示来减少乘法的次数
   *
   * !!注意, "MonoidOperation"必须满足结合律, 但不需要满足交换律
   */
  template <class T, class Integer, class MonoidOperation>
  T power(T x, Integer n, MonoidOperation op)
  {
    if (n == 0)
      return zfwstl::identity_element(op); // 取出"证同元素": 任何元素的0次幂都是恒等元素
    else
    {
      while ((n & 1) == 0) // 直到 n 变成奇数
      {
        n >>= 1;      // n 除以2 (通过位运算 n >>= 1)
        x = op(x, x); // 将 x 自乘
      }
      T result = x;
      n >>= 1;
      while (n != 0)
      {
        x = op(x, x);
        if ((n & 1) != 0)
          result = op(result, x);
        n >>= 1;
      }
      return result;
    }
  }
}

#endif // !ZFWSTL_NUMERIC_H_
