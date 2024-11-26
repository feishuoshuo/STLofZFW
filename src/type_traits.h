#ifndef ZFWSTL_TYPE_TRAITS_H_
#define ZFWSTL_TYPE_TRAITS_H_
// use standard header for type_traits
#include <type_traits>
// TODO:干什么用的
/**
 * 这个头文件用于提取类型信息
 * m_integral_constant 结构
 * m_bool_constant 类型别名
 * is_pair 类型特征
 */
namespace zfwstl
{
  // helper struct
  template <class T, T v>
  struct m_integral_constant // 帮助结构 这个结构模仿了 C++11 中的 std::integral_constant
  {
    // static constexpr静态常量表达式
    // 它们在编译时可以被求值，并且结果可以用于常量表达式
    static constexpr T value = v;
  };

  template <bool b>
  using m_bool_constant = m_integral_constant<bool, b>;

  typedef m_bool_constant<true> m_true_type;
  typedef m_bool_constant<false> m_false_type;

  /*****************************************************************************************/
  // type traits

  // is_pair 类型特征模板，它用于检查一个类型是否为 pair 类型

  // --- forward declaration begin
  template <class T1, class T2>
  struct pair;
  // --- forward declaration end

  template <class T>
  struct is_pair : zfwstl::m_false_type
  {
  };

  // 当模板参数 T 是 zfwstl::pair<T1, T2> 类型时，is_pair<T>::value 将是一个 true 常量。
  // 这意味着 is_pair 可以正确地识别 pair 类型
  template <class T1, class T2>
  struct is_pair<zfwstl::pair<T1, T2>> : zfwstl::m_true_type
  {
  }; // 特化
}

#endif // !ZFWSTLSTL_TYPE_TRAITS_H_