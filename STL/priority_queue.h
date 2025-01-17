#ifndef ZFWSTL_PRIO_QUEUE_H_
#define ZFWSTL_PRIO_QUEUE_H_
/**
 * 优先级队列(权值高先出)
 * 属于container adapter
 */
#include <cstddef> //for size_t, ptrdiff_t
#include "vector.h"
#include "../src/functional.h"           //for less
#include "../src/algorithms/heap_algo.h" //for push_heap, pop_heap, sort_heap, make_heap
#include "../src/iterator.h"             //for is_input_iterator
#include "../src/util.h"                 //for move()
namespace zfwstl
{

  template <class T, class Container = zfwstl::vector<T>, class Compare = zfwstl::less<typename Container::value_type>>
  class priority_queue
  {
  public:
    friend bool operator==(const priority_queue &lhs, const priority_queue &rhs) { return lhs.c == rhs.c; }
    friend bool operator!=(const priority_queue &lhs, const priority_queue &rhs) { return lhs.c != rhs.c; }

  public:
    typedef Container container_type;
    typedef Compare value_compare;
    // 使用底层容器的型别
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;
    static_assert(std::is_same<T, value_type>::value,
                  "the value_type of Container should be same with T");

  private:
    container_type c;   // 底层容器
    value_compare comp; // 权值比较的标准
  public:
    // 默认构造
    priority_queue() = default;
    // 参数化构造
    explicit priority_queue(const value_compare &x) : c(), comp(x) {}
    explicit priority_queue(size_type n)
        : c(n)
    {
      zfwstl::make_heap(c.begin(), c.end(), comp);
    }
    priority_queue(size_type n, const value_type &value) : c(n, value)
    {
      zfwstl::make_heap(c.begin(), c.end(), comp);
    }
    template <class InputIter, typename std::enable_if<
                                   zfwstl::is_input_iterator<InputIter>::value, int>::type = 0>
    priority_queue(InputIter first, InputIter last, const Compare &x) : c(first, last), comp(x) { zfwstl::make_heap(c.begin(), c.end(), comp); }
    template <class InputIter, typename std::enable_if<
                                   zfwstl::is_input_iterator<InputIter>::value, int>::type = 0>
    priority_queue(InputIter first, InputIter last) : c(first, last)
    {
      MYSTL_DEBUG(!(last < first));
      make_heap(c.begin(), c.end(), comp);
    }
    priority_queue(std::initializer_list<T> ilist)
        : c(ilist)
    {
      zfwstl::make_heap(c.begin(), c.end(), comp);
    }
    priority_queue(const Container &s)
        : c(s)
    {
      zfwstl::make_heap(c.begin(), c.end(), comp);
    }
    priority_queue(Container &&s)
        : c(zfwstl::move(s))
    {
      zfwstl::make_heap(c.begin(), c.end(), comp);
    }
    priority_queue(const priority_queue &rhs)
        : c(rhs.c), comp(rhs.comp)
    {
      zfwstl::make_heap(c.begin(), c.end(), comp);
    }
    priority_queue(priority_queue &&rhs)
        : c(zfwstl::move(rhs.c)), comp(rhs.comp)
    {
      zfwstl::make_heap(c.begin(), c.end(), comp);
    }
    ~priority_queue() = default;
    //=================operator操作运算符重载=====================
    priority_queue &operator=(const priority_queue &rhs)
    {
      c = rhs.c;
      comp = rhs.comp;
      zfwstl::make_heap(c.begin(), c.end(), comp);
      return *this;
    }
    priority_queue &operator=(priority_queue &&rhs)
    {
      c = zfwstl::move(rhs.c);
      comp = rhs.comp;
      zfwstl::make_heap(c.begin(), c.end(), comp);
      return *this;
    }
    priority_queue &operator=(std::initializer_list<T> ilist)
    {
      c = ilist;
      comp = value_compare();
      zfwstl::make_heap(c.begin(), c.end(), comp);
      return *this;
    }

    // 以下完全利用底层容器 c的操作完成
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    size_type max_size() const { return c.max_size(); }
    reference top() { return c.front(); }
    const_reference top() const { return c.front(); }

    void push(const value_type &x)
    {
      try
      {
        c.push_back(x);
        zfwstl::push_heap(c.begin(), c.end(), comp);
      }
      catch (...)
      {
        c.clear();
      }
    }
    void push(const value_type &&x)
    {
      try
      {
        c.push_back(zfwstl::move(x));
        zfwstl::push_heap(c.begin(), c.end(), comp);
      }
      catch (...)
      {
        c.clear();
      }
    }
    void pop()
    {
      try
      {
        pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
      }
      catch (...)
      {
        c.clear();
      }
    }
    template <class... Args>
    void emplace(Args &&...args)
    {
      c.emplace_back(zfwstl::forward<Args>(args)...);
      zfwstl::push_heap(c.begin(), c.end(), comp);
    }
    void clear()
    {
      while (!empty())
        pop();
    }
    void swap(priority_queue &rhs) noexcept(noexcept(zfwstl::swap(c, rhs.c)) &&
                                            noexcept(zfwstl::swap(comp, rhs.comp)))
    {
      zfwstl::swap(c, rhs.c);
      zfwstl::swap(comp, rhs.comp);
    }
  };

  template <class T, class Container, class Compare>
  bool operator==(priority_queue<T, Container, Compare> &lhs,
                  priority_queue<T, Container, Compare> &rhs)
  {
    return lhs == rhs;
  }

  template <class T, class Container, class Compare>
  bool operator!=(priority_queue<T, Container, Compare> &lhs,
                  priority_queue<T, Container, Compare> &rhs)
  {
    return lhs != rhs;
  }

  // 重载 zfwstl 的 swap
  template <class T, class Container, class Compare>
  void swap(priority_queue<T, Container, Compare> &lhs,
            priority_queue<T, Container, Compare> &rhs) noexcept(noexcept(lhs.swap(rhs)))
  {
    lhs.swap(rhs);
  }

}

#endif // !ZFWSTL_PRIO_QUEUE_H_