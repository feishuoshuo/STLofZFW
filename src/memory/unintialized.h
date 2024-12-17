#ifndef ZFWSTL_UNINTIALIZED_H_
#define ZFWSTL_UNINTIALIZED_H_
#include "../iterator.h"             //for value_type()迭代器类型萃取
#include <type_traits>               //for is_trivially_copy_assignable, true_type, false_type
#include "construct.h"               // for construct(), destroy()
#include "../algorithms/algorithm.h" //for algobase.h 's fill_n(), fill()
#include "../algorithms/algobase.h"  //for move()
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
  template <class ForwardIter, class T>
  void
  unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T &value, std::true_type)
  {
    zfwstl::fill(first, last, value);
  }

  template <class ForwardIter, class T>
  void
  unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T &value, std::false_type)
  {
    auto cur = first;
    try
    {
      for (; cur != last; ++cur)
      {
        zfwstl::construct(&*cur, value);
      }
    }
    catch (...)
    {
      for (; first != cur; ++first)
        zfwstl::destroy(&*first);
    }
  }

  template <class ForwardIter, class T>
  void uninitialized_fill(ForwardIter first, ForwardIter last, const T &value)
  {
    zfwstl::unchecked_uninit_fill(first, last, value,
                                  std::is_trivially_copy_assignable<
                                      typename iterator_traits<ForwardIter>::
                                          value_type>{});
  }
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
  //=====================uninitialized_move=====================
  // 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
  template <class InputIter, class ForwardIter>
  ForwardIter
  unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type)
  {
    return zfwstl::move(first, last, result);
  }

  template <class InputIter, class ForwardIter>
  ForwardIter
  unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type)
  {
    ForwardIter cur = result;
    try
    {
      for (; first != last; ++first, ++cur)
      {
        zfwstl::construct(&*cur, zfwstl::move(*first));
      }
    }
    catch (...)
    {
      zfwstl::destroy(result, cur);
    }
    return cur;
  }

  template <class InputIter, class ForwardIter>
  ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result)
  {
    return zfwstl::unchecked_uninit_move(first, last, result,
                                         std::is_trivially_move_assignable<
                                             typename iterator_traits<InputIter>::
                                                 value_type>{});
  }
}
#endif // !ZFWSTLSTL_UNINTIALIZED_H_