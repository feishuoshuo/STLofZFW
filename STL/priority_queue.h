#ifndef ZFWSTL_PRIO_QUEUE_H_
#define ZFWSTL_PRIO_QUEUE_H_
/**
 * 优先级队列(权值高先出)
 * 属于container adapter
 */
#include <cstddef> //for size_t, ptrdiff_t
#include "vector.h"
#include "../src/functional.h"
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
  };

}

#endif // !ZFWSTL_PRIO_QUEUE_H_