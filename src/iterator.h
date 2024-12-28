#ifndef ZFWSTL_TYPE_ITERATOR_H_
#define ZFWSTL_TYPE_ITERATOR_H_
/**
 * 迭代器的设计
 */
#include <cstddef>       // for ptrdiff_t
#include "type_traits.h" //for m_bool_constant
#include <iostream>
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
  // 用于检测一个类型 T 是否定义了 iterator_category 类型。
  /**
   * 这个结构体使用了SFINAE（Substitution Failure Is Not An Error）技术，
   * 这是一种在模板元编程中常用的技术，用于在编译时根据类型特征来决定模板的选择
   */
  template <class T>
  struct has_iterator_cat
  {
  private:
    struct two
    {
      char a;
      char b;
    };
    template <class U>
    static two test(...);
    template <class U>
    static char test(typename U::iterator_category * = 0);

  public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char); // 表示 T 是否包含 iterator_category
  };

  // 部分特化的模板，用于提供迭代器的特性
  template <class Iterator, bool>
  struct iterator_traits_impl
  {
  };

  template <class Iterator>
  struct iterator_traits_impl<Iterator, true>
  {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::difference_type difference_type;
  };

  template <class Iterator, bool>
  struct iterator_traits_helper
  {
  };

  // 根据迭代器的类别来提供正确的特性
  //  使用 std::is_convertible 检查 iterator_category 是否可以转换为 input_iterator_tag 或 output_iterator_tag
  template <class Iterator>
  struct iterator_traits_helper<Iterator, true>
      : public iterator_traits_impl<Iterator,
                                    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
                                        std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
  {
  };

  // 萃取迭代器的特性
  template <class Iterator>
  struct iterator_traits
      : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>
  {
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

  template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
  struct has_iterator_cat_of
      : public m_bool_constant<std::is_convertible<
            typename iterator_traits<T>::iterator_category, U>::value>
  {
  };
  // ====================萃取某种迭代器====================
  template <class T, class U>
  struct has_iterator_cat_of<T, U, false> : public m_false_type
  {
  };

  template <class Iter>
  struct is_exactly_input_iterator : public m_bool_constant<has_iterator_cat_of<Iter, input_iterator_tag>::value &&
                                                            !has_iterator_cat_of<Iter, forward_iterator_tag>::value>
  {
  };

  template <class Iter>
  struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag>
  {
  };

  template <class Iter>
  struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag>
  {
  };

  template <class Iter>
  struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag>
  {
  };

  template <class Iter>
  struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag>
  {
  };

  template <class Iter>
  struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag>
  {
  };

  template <class Iterator>
  struct is_iterator : public m_bool_constant<is_input_iterator<Iterator>::value ||
                                              is_output_iterator<Iterator>::value>
  {
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

  // 萃取某个迭代器的 distance_type
  template <class Iterator>
  typename iterator_traits<Iterator>::difference_type *
  distance_type(const Iterator &)
  {
    return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
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
  //=====================iterator adapters迭代器配接器=====================
  /**
   * Insert Iterators
   * 用来将某个迭代器的赋值(assign)操作修改为插入(insert)操作
   * 1. back_insert从容器尾端插入
   * 2. front_insert从容器头部插入
   * 3. insert_iterator在指定位置上进行，插入后将迭代器右移一个位置
   */
  template <class Container>
  class back_insert_iterator
  {
  protected:
    Container *container; // 底层容器
  public:
    typedef output_iterator_tag iterator_category; // 注意类型
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    // 该构造函数使 back_insert_iterator 与容器绑定起来！！
    explicit back_insert_iterator(Container &x) : container(&x) {}
    back_insert_iterator<Container> &operator=(const typename Container::value_type &value)
    {
      container->push_back(value); // 关键！转而调用push_back()
      return *this;
    }
    // NOTE: Insert Iterators的前进、后退、取值、成员取用等操作都是没有意义的，甚至不被允许
    //(关闭功能)，均返回对象自己
    back_insert_iterator<Container> &operator*() { return *this; }
    back_insert_iterator<Container> &operator++() { return *this; }
    back_insert_iterator<Container> &operator++(int) { return *this; }
  };

  template <class Container>
  class front_insert_iterator
  {
  protected:
    Container *container; // 底层容器
  public:
    typedef output_iterator_tag iterator_category; // 注意类型
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    explicit front_insert_iterator(Container &x) : container(&x) {}
    front_insert_iterator<Container> &operator=(const typename Container::value_type &value)
    {
      container->push_front(value); // 关键！转而调用push_front()
      return *this;
    }
    // Insert Iterators的前进、后退、取值、成员取用等操作都是没有意义的，甚至不被允许
    //(关闭功能)，均返回对象自己
    front_insert_iterator<Container> &operator*() { return *this; }
    front_insert_iterator<Container> &operator++() { return *this; }
    front_insert_iterator<Container> &operator++(int) { return *this; }
  };

  template <class Container>
  class insert_iterator
  {
  protected:
    Container *container; // 底层容器
    typename Container::iterator iter;

  public:
    typedef output_iterator_tag iterator_category; // 注意类型
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    insert_iterator(Container &x, typename Container::iterator i) : container(&x), iter(i) {}
    insert_iterator<Container> &operator=(const typename Container::value_type &value)
    {
      iter = container->insert(iter, value); // 关键！！转调用insert()
      ++iter;                                // ！！使得insert iterator永远随1其目标贴身移动
      return *this;
    }
    // Insert Iterators的前进、后退、取值、成员取用等操作都是没有意义的，甚至不被允许
    //(关闭功能)，均返回对象自己
    insert_iterator<Container> &operator*() { return *this; }
    insert_iterator<Container> &operator++() { return *this; }
    insert_iterator<Container> &operator++(int) { return *this; }
  };

  // 辅助函数，帮助我们方便使用insert_iterator相关迭代器配置器
  template <class Container>
  inline back_insert_iterator<Container> back_inserter(Container &x) { return back_insert_iterator<Container>(x); }

  template <class Container>
  inline front_insert_iterator<Container> front_inserter(Container &x) { return front_insert_iterator<Container>(x); }

  template <class Container, class Iterator>
  inline insert_iterator<Container> inserter(Container &x, Iterator i)
  {
    typedef typename Container::iterator iter;
    return insert_iterator<Container>(x, iter(i)); // iter(i)隐式迭代器的类型转换
  }

  /**
   * Reverse Iterators
   * 代表反向迭代器，使前进为后退，后退为前进
   */
  template <class Iterator>
  class reverse_iterator
  {
  protected:
    Iterator current; // 记录对应之正向迭代器
  public:
    // 反向迭代器的五种相应型别, 都和其对应的正向迭代器相同
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::pointer pointer;
    typedef typename iterator_traits<Iterator>::reference reference;

    typedef Iterator iterator_type;          // 代表正向迭代器
    typedef reverse_iterator<Iterator> self; // 代表反向迭代器
  public:
    reverse_iterator() {}
    // 下面构造函数将 reverse_iterator 与某个正向迭代器 x关联起来
    explicit reverse_iterator(iterator_type x) : current(x) {}
    reverse_iterator(const self &x) : current(x.current) {}

    iterator_type base() const { return current; } // 取出对应的正向迭代器

    reference operator*() const
    {
      Iterator tmp = current;
      return *--tmp; //!!
    }
    pointer operator->() const { return &(operator*()); } // 同上
    self &operator++()
    {
      --current;
      return *this;
    }
    self &operator--()
    {
      ++current;
      return *this;
    }
    self &operator++(int)
    {
      self tmp = *this;
      --current;
      return tmp;
    }
    self &operator--(int)
    {
      self tmp = *this;
      ++current;
      return tmp;
    }
    // 计算两个反向迭代器之间的距离
    difference_type operator-(const reverse_iterator &rhs) const
    {
      // 因为反向迭代器的current成员变量是指向对应正向迭代器的，
      // 所以直接计算两个正向迭代器之间的距离即可。
      return this->current - rhs.current;
    }
    // 前进与后退方向完全逆转
    // TAG: 这里的const是一个常量成员函数。这意味着函数不会修改类的任何成员变量，也就是说，它不会改变对象的状态
    self operator+(difference_type n) const { return self(current - n); }
    self operator-(difference_type n) const { return self(current + n); }
    self operator+=(difference_type n)
    {
      current -= n;
      return *this;
    }
    self operator-=(difference_type n)
    {
      current -= n;
      return *this;
    }
    //(*this + n)回调哦那个本类的operator*和operator+; 最外面的*不会
    reference operator[](difference_type n) const { return *(*this + n); }
  };

  /**
   * IOStream Iterators
   * 1. isteram iterator !!永远在最必要时，才定义一个isteram iterator
   *    是一个 input iterator
   * 2. ostream iterator
   *    是一个 output iterator
   */
  template <class T, class Distance = ptrdiff_t>
  class istream_iterator
  {
    friend bool operator==(const istream_iterator<T, Distance> &a, const istream_iterator<T, Distance> &b)
    {
      if (a.end_marker && b.end_marker)
        return true; // 如果两个迭代器都到达了流的末尾，则它们相等
      if (!a.end_marker && !b.end_marker)
        return false; // 如果两个迭代器都未到达流的末尾，则它们不相等
      return false;   // 一个到达了末尾而另一个没有，或者一个有效而另一个无效，则它们不相等
    }
    friend bool operator!=(const istream_iterator<T, Distance> &a, const istream_iterator<T, Distance> &b) { return !(a == b); }

  protected:
    std::istream *stream; // 内部维护了一个istream member
    T value;
    bool end_marker;

    // 目的是用来检测流的结束或者读取错误，并读取数据
    void read()
    {
      end_marker = (*stream) ? true : false;
      if (end_marker)
        *stream >> value; // ！！关键！！
      // 上述输入之后，stream状态可能改变，所以下面再做一次以决定 end_marker
      // 当读到eof或读到型别不符的资料，stream即处于false状态
      end_marker = (*stream) ? true : false;
    }

  public:
    typedef input_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    // 因此身为input iterator，所以采用const比较保险
    typedef const T *pointer;
    typedef const T &reference;

    istream_iterator() : stream(&std::cin), end_marker(false) {}
    istream_iterator(std::istream &s) : stream(&s) { read(); }

    reference operator*() const { return value; }
    pointer operator->() const { return &(operator*()); }
    // 迭代器前进一个位置，就代表要读取一笔资料
    istream_iterator<T, Distance> &operator++()
    {
      read();
      return *this;
    }
    istream_iterator<T, Distance> &operator++(int)
    {
      istream_iterator<T, Distance> tmp = *this;
      read();
      return tmp;
    }
  };

  template <class T>
  class ostream_iterator
  {
    // friend bool operator==
  protected:
    std::ostream *stream; // 内部维护了一个ostream member
    const char *string;   // 每次输出后的间隔符号，变量名称可以叫string!
  public:
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    ostream_iterator(std::ostream &s) : stream(&std::cout), string(0) {}
    // 下面构造函数用法例子：ostream_iterator<int> outiter(cout, ' ');
    ostream_iterator(std::ostream &s, const char *c) : stream(&s), string(c) {}
    // 对迭代器做赋值(assign)操作，就代表要输出一笔资料
    ostream_iterator<T> &operator=(const T &value)
    {
      *stream << value; //!!关键！！输出数值
      if (string)
        *stream << string; // 如果输出状态无误，输出间隔符号
      return *this;
    }

    ostream_iterator<T> &operator*() { return *this; }
    ostream_iterator<T> &operator++() { return *this; }
    ostream_iterator<T> &operator++(int) { return *this; }
  };

}
#endif // !ZFWSTLSTL_TYPE_ITERATOR_H_