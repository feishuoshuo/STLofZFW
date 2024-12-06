#ifndef ZFWSTL_QUEUE_H_
#define ZFWSTL_QUEUE_H_
/**
 * 队列(FIFO 先进先出)
 * 属于container adapter
 */
#include <cstddef> //for size_t, ptrdiff_t
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
    // 构造、复制、移动函数
    // TODO:可以完善
    queue() = default;
    explicit queue(size_type n) : c(n) {}
    queue(size_type n, const value_type &value) : c(n, value) {}
    template <class IIter>
    queue(IIter first, IIter last) : c(first, last) {}
    queue(const Container &c) : c(c) {}

    ~queue() = default;

  public:
    // 以下完全利用底层容器 c的操作完成
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    size_type max_size() const { return c.max_size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }

    void push(const value_type &x) { c.push_back(x); }
    void pop() { c.pop_front(); }
  };
  template <class T, class Container>
  bool operator==(const queue<T, Container> &x, const queue<T, Container> &y)
  {
    return x.c == y.c;
  }
  template <class T, class Container>
  bool operator<(const queue<T, Container> &x, const queue<T, Container> &y)
  {
    return x.c < y.c;
  }
}

#endif // !ZFWSTL_QUEUE_H_