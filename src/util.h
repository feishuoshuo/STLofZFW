#ifndef ZFWSTL_UTIL_H_
#define ZFWSTL_UTIL_H_
/**
 * 包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等
 * */
// TODO:研究这些的作用
#include <cstddef> //for size_t
#include "type_traits.h"
namespace zfwstl
{
  // ==========================move========================

  template <class T>
  typename std::remove_reference<T>::type &&move(T &&arg) noexcept
  {
    return static_cast<typename std::remove_reference<T>::type &&>(arg);
  }
  // =======================swap==========================
  template <class Tp>
  void swap(Tp &lhs, Tp &rhs)
  {
    auto tmp(zfwstl::move(lhs));
    lhs = zfwstl::move(rhs);
    rhs = zfwstl::move(tmp);
  }
  // template <class ForwardIter1, class ForwardIter2>
  // ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
  // {
  //   for (; first1 != last1; ++first1, (void)++first2)
  //     zfwstl::swap(*first1, *first2);
  //   return first2;
  // }

  // template <class Tp, size_t N>
  // void swap(Tp (&a)[N], Tp (&b)[N])
  // {
  //   zfwstl::swap_range(a, a + N, b);
  // }
}
#endif // !ZFWSTL_UTIL_H_