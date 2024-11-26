#ifndef ZFWSTL_TYPE_ITERATOR_H_
#define ZFWSTL_TYPE_ITERATOR_H_
/**
 * 迭代器的设计
 */
#include <cstddef> // for ptrdiff_t

namespace zfwstl
{
  // 五种迭代器类型
  struct input_iterator_tag
  {
  };
  struct output_iterator_tag
  {
  };
  struct forward_iterator_tag : public input_iterator_tag
  {
  };
  struct bidirectional_iterator_tag : public forward_iterator_tag
  {
  };
  struct random_access_iterator_tag : public bidirectional_iterator_tag
  {
  };

  // iterator 模板
  template <class Category, class T, class Distance = ptrdiff_t,
            class Pointer = T *, class Reference = T &>
  struct iterator
  {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Pointer pointer;
    typedef Reference reference;
    typedef Distance difference_type;
  };

  // iterator traits
  template <class Iterator>
  struct iterator_traits
  {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
  };

  // =========================traits偏特化版本=========================
  //  针对原生指针native pointer的
  template <class T>
  struct iterator_traits<T *>
  {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_t difference_type;
  };
  // 针对原生pointer-to-const的
  template <class T>
  struct iterator_traits<const T *>
  {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_t difference_type; // 迭代器之间距离
  };

  // =========================迭代器相应型别萃取函数=====================
  template <class Iterator>
  inline typename iterator_traits<Iterator>::iterator_category
  iterator_category(const Iterator &)
  {
    /**
     * 返回一个类型实例可以直接用于类型检查和类型比较，而不需要额外的指针操作 */
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category(); // 暂时对象 用于分类迭代器的
  }

  template <class Iterator>
  inline typename iterator_traits<Iterator>::value_type *
  value_type(const Iterator &)
  {
    // TAG: static_cast<typename iterator_traits<Iterator>::value_type *>(0);
    /**
     * 返回了一个指向difference_type类型的空指针（nullptr）。
     * 这种写法通常用于编译时的类型检查，而不是实际的运行时操作。
     * 通过尝试将函数的返回值转换为difference_type的指针，
     * 编译器可以检查是否存在这样的类型，并在不存在时产生编译错误
     */
    return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
  }

  template <class Iterator>
  inline typename iterator_traits<Iterator>::difference_type *
  difference_type(const Iterator &)
  {
    return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
  }

  // =========================以下函数用于计算迭代器间的距离==============
  // distance 的 input_iterator_tag 的版本
  template <class InputIterator>
  inline typename iterator_traits<InputIterator>::difference_type
  __distance(InputIterator first, InputIterator last, input_iterator_tag)
  {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
    {
      ++first;
      ++n;
    }
    return n;
  }

  // distance 的 random_access_iterator_tag 的版本
  template <class RandomIter>
  inline typename iterator_traits<RandomIter>::difference_type
  __distance(RandomIter first, RandomIter last, random_access_iterator_tag)
  {
    return last - first;
  }

  template <class InputIterator>
  inline typename iterator_traits<InputIterator>::difference_type
  distance(InputIterator first, InputIterator last)
  {
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return __distance(first, last, category());
  }

  // ====================以下函数用于让迭代器前进 n 个距离===========
  // advance 的 input_iterator_tag 的版本
  template <class InputIterator, class Distance>
  inline void __advance(InputIterator &i, Distance n, input_iterator_tag)
  {
    while (n--)
      ++i;
  }

  // advance 的 bidirectional_iterator_tag 的版本
  template <class BidirectionalIterator, class Distance>
  inline void __advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag)
  {
    if (n >= 0)
      while (n--)
        ++i;
    else
      while (n++)
        --i;
  }

  // advance 的 random_access_iterator_tag 的版本
  template <class RandomIter, class Distance>
  inline void __advance(RandomIter &i, Distance n, random_access_iterator_tag)
  {
    i += n;
  }

  template <class InputIterator, class Distance>
  inline void advance(InputIterator &i, Distance n)
  {
    __advance(i, n, iterator_category(i));
  }

  // 模板类 : reverse_iterator
  // TODO:代表反向迭代器，使前进为后退，后退为前进
}
#endif // !ZFWSTLSTL_TYPE_ITERATOR_H_