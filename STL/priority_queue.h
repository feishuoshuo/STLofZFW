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
namespace zfwstl
{

  template <class T, class Container = zfwstl::vector<T>, class Compare = zfwstl::less<typename Container::value_type>>
  class priority_queue
  {
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
    // 构造、复制、移动函数
    // TODO:可以完善
    priority_queue() : c() {}
    explicit priority_queue(const value_compare &x) : c(), comp(x) {}
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
        push_heap(c.begin(), c.end(), comp);
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
  };

}

#endif // !ZFWSTL_PRIO_QUEUE_H_