#ifndef ZFWSTL_STACK_H_
#define ZFWSTL_STACK_H_
/**
 * 栈(FILO先进后出)
 * 属于container adapter
 */
#include <cstddef>       //for size_t, ptrdiff_t
#include "../src/util.h" //for move()
#include "deque.h"
namespace zfwstl
{

  template <class T, class Container = zfwstl::deque<T>>
  class stack
  {
    friend bool operator==(const stack &lhs, const stack &rhs) { return lhs.c == rhs.c; }
    friend bool operator<(const stack &lhs, const stack &rhs) { return lhs.c < rhs.c; }

  public:
    typedef Container container_type;
    // 使用底层容器的型别
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

  private:
    container_type c; // 底层容器
  public:
    // 默认构造
    stack() = default;
    // 参数化构造
    explicit stack(size_type n) : c(n) {}
    stack(size_type n, const value_type &value) : c(n, value) {}
    template <class IIter>
    stack(IIter first, IIter last) : c(first, last) {}
    // 初始化列表构造
    stack(std::initializer_list<T> ilist) : c(ilist.begin(), ilist.end()) {}
    // 拷贝构造
    stack(const Container &c) : c(c) {}
    stack(const stack &rhs) : c(rhs.c) {}
    // 移动构造
    stack(Container &&c) noexcept(std::is_nothrow_move_constructible<Container>::value) : c(zfwstl::move(c)) {}
    stack(stack &&rhs) noexcept(std::is_nothrow_move_constructible<Container>::value) : c(zfwstl::move(rhs.c)) {}
    // 默认析构
    ~stack() = default;

  public:
    //=================operator操作运算符重载=====================
    stack &operator=(const stack &rhs)
    {
      c = rhs.c;
      return *this;
    }
    stack &operator=(stack &&rhs) noexcept(std::is_nothrow_move_assignable<Container>::value)
    {
      c = zfwstl::move(rhs.c);
      return *this;
    }
    stack &operator=(std::initializer_list<T> ilist)
    {
      c = ilist;
      return *this;
    }

    // 以下完全利用底层容器 c的操作完成
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    size_type max_size() const { return c.max_size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }

    template <class... Args>
    void emplace(Args &&...args) { c.emplace_back(zfwstl::forward<Args>(args)...); }

    void push(const value_type &x) { c.push_back(x); }
    void push(const value_type &&x) { c.push_back(zfwstl::move(x)); }
    void pop() { c.pop_back(); }
    void clear()
    {
      while (!empty())
        pop();
    }
    void swap(stack &rhs) noexcept(noexcept(zfwstl::swap(c, rhs.c)))
    {
      zfwstl::swap(c, rhs.c);
    }
  };

  template <class T, class Container>
  bool operator==(const stack<T, Container> &x, const stack<T, Container> &y)
  {
    return x == y;
  }
  template <class T, class Container>
  bool operator!=(const stack<T, Container> &x, const stack<T, Container> &y)
  {
    return !(x == y);
  }
  template <class T, class Container>
  bool operator>(const stack<T, Container> &x, const stack<T, Container> &y)
  {
    return y < x;
  }
  template <class T, class Container>
  bool operator<(const stack<T, Container> &x, const stack<T, Container> &y)
  {
    return x < y;
  }
  template <class T, class Container>
  bool operator<=(const stack<T, Container> &x, const stack<T, Container> &y)
  {
    return !(y < x);
  }
  template <class T, class Container>
  bool operator>=(const stack<T, Container> &x, const stack<T, Container> &y)
  {
    return !(x < y);
  }

  // 重载 zfwstl 的 swap
  template <class T, class Container>
  void swap(stack<T, Container> &lhs, stack<T, Container> &rhs) noexcept(noexcept(lhs.swap(rhs)))
  {
    lhs.swap(rhs);
  }
}

#endif // !ZFWSTL_STACK_H_