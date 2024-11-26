#ifndef ZFWSTL_ALLOCATOR_H_
#define ZFWSTL_ALLOCATOR_H_
/**
 * 标准的空间配置器
 */
#include <cstddef>     //for size_t, ptrdiff_t
#include "construct.h" // for zfwstl::construct, zfwstl::destroy
namespace zfwstl
{
  template <class T>
  class simple_allocator
  {
  public:
    typedef T value_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

  public:
    // 内存空间的配置与释放
    static T *allocate()
    {
      // TAG: static_cast类型转换，将::operator new返回的void*指针转换为T*类型的指针
      return static_cast<T *>(::operator new(sizeof(T)));
    }
    static T *allocate(size_type n)
    {
      if (0 == n)
        return nullptr;
      return static_cast<T *>(::operator new(n * sizeof(T)));
    }

    static void deallocate(T *ptr)
    {
      if (ptr == nullptr)
        return;
      ::operator delete(ptr);
    }
    static void deallocate(T *ptr, size_type /* n */)
    {
      if (ptr == nullptr)
        return;
      ::operator delete(ptr);
    }
    // 对象内容的构造、析构
    static void construct(T *ptr)
    {
      zfwstl::construct(ptr);
    }
    static void construct(T *ptr, const T &value)
    {
      zfwstl::construct(ptr, value);
    }
    // TODO:左值引用
    static void construct(T *ptr, T &&value);

    template <class... Args>
    static void construct(T *ptr, Args &&...args)
    {
      // TODO: 需要在util.h文件中编写forward模板才能调用
      std::cout << "this is making..." << std::endl;
      //  mystl::construct(ptr, mystl::forward<Args>(args)...);
    }

    static void destroy(T *ptr)
    {
      zfwstl::destroy(ptr);
    }
    static void destroy(T *first, T *last)
    {
      zfwstl::destroy(first, last);
    }
  };
}

#endif // !ZFWSTLSTL_ALLOCATOR_H_