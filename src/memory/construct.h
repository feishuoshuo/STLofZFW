#ifndef ZFWSTL_CONSTRUCTOR_H_
#define ZFWSTL_CONSTRUCTOR_H_
/**
 * 对象内容的构造、析构
 * construct : 负责对象的构造
 * destroy   : 负责对象的析构
 */
#include <cstddef>   // for size_t, ptrdiff_t, nullptr_t
#include <new>       // for placement new
#include <cstdlib>   // for exit()
#include <climits>   // for UINT_MAX
#include <iostream>  // for cerr
#include "../util.h" //for forward
using namespace std;

namespace zfwstl
{
  // 1.construct 构造对象
  template <class T1, class T2>
  inline void construct(T1 *_p, const T2 &_value)
  {
    ::new ((void *)_p) T1(_value);
  }

  template <class T1>
  inline void construct(T1 *_p)
  {
    ::new ((void *)_p) T1(); // 调用 T1 类型的默认构造函数
  }

  template <class Ty, class... Args>
  void construct(Ty *ptr, Args &&...args)
  {
    ::new ((void *)ptr) Ty(zfwstl::forward<Args>(args)...);
  }

  // 2.destroy 将对象析构 [优化：是否为平凡trivial函数]
  // NOTE:trivial destructor 是指那些不执行任何操作的析构函数
  /**
   * 由于这些类型的析构函数是平凡的，它们不需要任何特别的销毁逻辑。
   * 因此，当 trivial destructor 函数被调用时，不需要执行任何操作，
   * 因为这些类型的“销毁”仅仅是释放它们所占用的内存，而不需要调用析构函数。
   * 在这种情况下，trivial destructor 函数的特化版本被定义为空函数体，以优化性能，避免不必要的函数调用。
   *
   * 这种特化处理是合理的，因为它避免了对不需要析构的对象进行不必要的函数调用，从而提高了程序的效率。
   * 在实际的 STL 实现中，这种优化可以减少模板实例化的开销，并提高容器操作的性能。
   */
  // 版本一：接受一个指针
  template <class T1>
  inline void destroy_one(T1 *_p, std::true_type) {}

  template <class T1>
  inline void destroy_one(T1 *_p, std::false_type)
  {
    if (_p != nullptr)
      _p->~T1(); // 调用dtor ~T()
  }

  // 版本二：接受两个迭代器，析构掉[first, last)范围内对象
  template <class ForwardIter>
  inline void destroy_cat(ForwardIter _first, ForwardIter _last, std::true_type) {}

  template <class ForwardIter>
  inline void destroy_cat(ForwardIter _first, ForwardIter _last, std::false_type)
  {
    for (; _first != _last; ++_first)
      destroy(&*_first);
  }

  template <class T>
  void destroy(T *_p)
  { // NOTE: std::is_trivially_destructible<T>{}相当于创建了一个模板结构的实例
    // TAG: destroy_one(_p, std::is_trivially_destructible<T>{});
    // TODO: 我想尝试不创建实例，而是直接调用其value静态成员返回布尔值
    destroy_one(_p, std::is_trivially_destructible<T>{});
  }

  template <class ForwardIter>
  void destroy(ForwardIter _first, ForwardIter _last)
  {
    // typename iterator_traits<ForwardIter>::value_type 用到了迭代器的类型萃取
    destroy_cat(_first, _last, std::is_trivially_destructible<typename iterator_traits<ForwardIter>::value_type>{});
  }
}

#endif // !ZFWSTLSTL_CONSTRUCTOR_H_