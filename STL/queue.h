#ifndef ZFWSTL_QUEUE_H_
#define ZFWSTL_QUEUE_H_
/**
 * 队列(FIFO 先进先出)
 * 属于container adapter
 */
#include <cstddef>       //for size_t, ptrdiff_t
#include "../src/util.h" //for move()
#include "deque.h"
namespace zfwstl
{
  template <class T, class Container = zfwstl::deque<T>>
  class queue
  {
    friend bool operator==(const queue &lhs, const queue &rhs) { return lhs.c == rhs.c; }
    friend bool operator<(const queue &lhs, const queue &rhs) { return lhs.c < rhs.c; }

  public:
    typedef Container container_type;
    // 使用底层容器的型别
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;
    static_assert(std::is_same<T, value_type>::value,
                  "the value_type of Container should be same with T");

  private:
    container_type c; // 底层容器
  public:
    // 默认构造
    // 参数化构造
    queue() = default;
    explicit queue(size_type n) : c(n) {}
    queue(size_type n, const value_type &value) : c(n, value) {}
    template <class IIter>
    queue(IIter first, IIter last) : c(first, last) {}
    // 初始化列表构造
    queue(std::initializer_list<T> ilist) : c(ilist.begin(), ilist.end()) {}
    // 拷贝构造
    queue(const Container &c) : c(c) {}
    queue(const queue &rhs) : c(rhs.c) {}
    // 移动构造
    // TAG:std::is_nothrow_move_constructible<Container>::value是一个编译时的表达式
    // 是类型特征类，用于检查类型Container是否具有无抛出异常的移动构造函数
    // ::value是一个布尔值，如果Container类型具有无抛出异常的移动构造函数
    queue(Container &&cc) noexcept(std::is_nothrow_move_constructible<Container>::value) : c(zfwstl::move(cc)) {}
    queue(queue &&rhs) noexcept(std::is_nothrow_move_constructible<Container>::value) : c(zfwstl::move(rhs.c)) {}
    // 默认析构
    ~queue() = default;

  public:
    //=================operator操作运算符重载=====================
    queue &operator=(const queue &rhs)
    {
      c = rhs.c;
      return *this;
    }
    queue &operator=(queue &&rhs) noexcept(std::is_nothrow_move_assignable<Container>::value)
    {
      c = zfwstl::move(rhs.c);
      return *this;
    }
    queue &operator=(std::initializer_list<T> ilist)
    {
      c = ilist;
      return *this;
    }

    // 以下完全利用底层容器 c的操作完成
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    size_type max_size() const { return c.max_size(); }
    // 访问元素相关操作
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }

    // 修改容器相关操作
    template <class... Args>
    void emplace(Args &&...args)
    {
      c.emplace_back(zfwstl::forward<Args>(args)...);
    }
    void push(const value_type &x) { c.push_back(x); }
    void push(value_type &&value) { c.emplace_back(zfwstl::move(value)); }
    void pop() { c.pop_front(); }
    void clear()
    {
      while (!empty())
        pop();
    }
    void swap(queue &rhs) noexcept(noexcept(zfwstl::swap(c, rhs.c)))
    {
      zfwstl::swap(c, rhs.c);
    }
  };

  template <class T, class Container>
  bool operator==(const queue<T, Container> &x, const queue<T, Container> &y)
  {
    return x == y;
  }
  template <class T, class Container>
  bool operator!=(const queue<T, Container> &x, const queue<T, Container> &y)
  {
    return !(x == y);
  }
  template <class T, class Container>
  bool operator>(const queue<T, Container> &x, const queue<T, Container> &y)
  {
    return y < x;
  }
  template <class T, class Container>
  bool operator>=(const queue<T, Container> &x, const queue<T, Container> &y)
  {
    return !(x < y);
  }
  template <class T, class Container>
  bool operator<=(const queue<T, Container> &x, const queue<T, Container> &y)
  {
    return !(y < x);
  }
  template <class T, class Container>
  bool operator<(const queue<T, Container> &x, const queue<T, Container> &y)
  {
    return x < y;
  }

  // 重载 zfwstl 的 swap
  template <class T, class Container>
  void swap(queue<T, Container> &lhs, queue<T, Container> &rhs) noexcept(noexcept(lhs.swap(rhs)))
  {
    lhs.swap(rhs);
  }
}

#endif // !ZFWSTL_QUEUE_H_