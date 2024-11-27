#ifndef ZFWSTL_UNINTIALIZED_H_
#define ZFWSTL_UNINTIALIZED_H_
<<<<<<< HEAD
#include "../iterator.h"             //for value_type()迭代器类型萃取
#include <type_traits>               //for is_trivially_copy_assignable, true_type, false_type
#include "construct.h"               // for zfwstl::construct
#include "../algorithms/algorithm.h" //for algobase.h 's fill_n
namespace zfwstl
{
  // =====================uninitialized_copy=================================
  // 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
  template <class InputIter, class ForwardIter>
  ForwardIter
  __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, std::true_type)
  {
    return zfwstl::copy(first, last, result);
  }

  template <class InputIter, class ForwardIter>
  ForwardIter
  __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, std::false_type)
  {
    auto cur = result;
    try
    {
      for (; first != last; ++first, ++cur)
      {
        // TODO：zfwstl::construct(&*cur, *first); 改成直接放入地址试一下
        zfwstl::construct(cur, *first);
      }
    }
    catch (...)
    {
      // TODO：zfwstl::destroy(&*cur); 改成直接放入地址试一下
      for (; result != cur; --cur)
        zfwstl::destroy(cur);
    }
    return cur;
  }

  template <class InputIter, class ForwardIter>
  ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
  {
    return zfwstl::__uninitialized_copy(first, last, result,
                                        std::is_trivially_copy_assignable<
                                            typename iterator_traits<ForwardIter>::
                                                value_type>{});
  }

  // =====================uninitialized_copy_n================================
  // 把 [first, first + n) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置

  // =====================uninitialized_fill=====================
  // 在 [first, last) 区间内填充元素值

  // =====================uninitialized_fill_n=====================
  // 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
  template <class ForwardIter, class Size, class T>
  inline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T &value, std::true_type)
  {
    return zfwstl::fill_n(first, n, value);
  }

  template <class ForwardIter, class Size, class T>
  inline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T &value, std::false_type)
  {
    auto cur = first;
    // NOTE: &*cur获取迭代器所指向对象的地址
    for (; n > 0; --n, ++cur)
    {
      // TODO：zfwstl::construct(&*cur, value); 改成直接放入地址试一下
      zfwstl::construct(cur, value);
    }
    return cur;
  }

  template <class ForwardIter, class Size, class T>
  inline ForwardIter
  uninitialized_fill_n(ForwardIter first, Size n, const T &value)
  {
    // TAG: std::is_trivially_copy_assignable 用于在编译时检查一个类型是否具有平凡（trivial）的复制赋值操作
    return zfwstl::__uninitialized_fill_n(first, n, value,
                                          std::is_trivially_copy_assignable<
                                              typename iterator_traits<ForwardIter>::
                                                  value_type>{});
  }
}
=======

>>>>>>> 032d1b48e96f41c8c40afbe26cf6db12f00cbeaf
#endif // !ZFWSTLSTL_UNINTIALIZED_H_