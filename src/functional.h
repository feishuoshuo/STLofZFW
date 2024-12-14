#ifndef ZFWINYSTL_FUNCTIONAL_H_
#define ZFWINYSTL_FUNCTIONAL_H_
/**
 * 包含了 mystl 的函数对象与哈希函数
 */
#include <cstddef> //for size_t
namespace zfwstl
{
  // 定义一元函数的参数型别和返回值型别
  template <class Arg, class Result>
  struct unarg_function
  {
    typedef Arg argument_type;
    typedef Result result_type;
  };

  // 定义二元函数的参数型别的返回值型别
  template <class Arg1, class Arg2, class Result>
  struct binary_function
  {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
  };

  //==========================函数对象===========================
  // 加法
  template <class T>
  struct plus : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x + y; }
  };

  // 减法
  template <class T>
  struct minus : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x - y; }
  };

  // 乘法
  template <class T>
  struct multiplies : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x * y; }
  };

  // 除法
  template <class T>
  struct divides : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x / y; }
  };

  // 模取
  template <class T>
  struct modulus : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x % y; }
  };

  // 否定
  template <class T>
  struct negate : public unarg_function<T, T>
  {
    T operator()(const T &x) const { return -x; }
  };

  template <typename Pair>
  struct select1st : public unarg_function<Pair, typename Pair::first_type>
  {
    typename Pair::first_type &operator()(const Pair &pair) const
    {
      return pair.first;
    }
  };

  // 证同元素
  template <class T>
  struct identity : public unarg_function<T, T>
  {
    const T &operator()(const T &x) const { return x; }
  };

  // 加法的证同元素
  template <class T>
  T identity_element(plus<T>) { return T(0); }

  // 乘法的证同元素
  template <class T>
  T identity_element(multiplies<T>) { return T(1); }

  // 等于
  template <class T>
  struct equal_to : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x == y; }
  };

  // 不等于
  template <class T>
  struct not_equal_to : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x != y; }
  };

  // 大于
  template <class T>
  struct greater : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x > y; }
  };

  // 小于
  template <class T>
  struct less : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x < y; }
  };

  // 大于等于
  template <class T>
  struct greater_equal : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x >= y; }
  };

  // 小于等于
  template <class T>
  struct less_equal : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x <= y; }
  };

  // 逻辑与
  template <class T>
  struct logical_and : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x && y; }
  };

  // 逻辑或
  template <class T>
  struct logical_or : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x || y; }
  };

  // 逻辑非
  template <class T>
  struct logical_not : public unarg_function<T, bool>
  {
    bool operator()(const T &x) const { return !x; }
  };
}
#endif // !ZFWINYSTL_FUNCTIONAL_H_