#ifndef ZFWSTL_SLIST_H_
#define ZFWSTL_SLIST_H_
/**
 * slist 单向链表
 * 1-特别提供了insert_after, erase_after
 * 2-不提供push_back, 仅提供push_front
 */
#include <cstddef>                   //for size_t, ptrdiff_t
#include "../src/iterator.h"         //for forward_iterator_tag
#include "../src/memory/allocator.h" //标准空间配置器
namespace zfwstl
{
  struct __slist_node_base
  {
    __slist_node_base *next;
  };
  template <class T>
  struct __slist_node : public __slist_node_base
  {
    T data;
  };
  // 全局函数：已知某一节点，插入新节点于其后
  inline __slist_node_base *__slist_make_link(__slist_node_base *prev_node, __slist_node_base *new_node)
  {
    new_node->next = prev_node->next;
    prev_node->next = new_node;
    return new_node;
  }
  // 全局函数：单链表大小(元素个数)
  inline size_t __slist_size(__slist_node_base *node)
  {
    size_t result = 0;
    for (; node != nullptr; node = node->next)
      ++result;
    return result;
  }

  struct __slist_iterator_base
  {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef forward_iterator_tag iterator_category; // 单向

    __slist_node_base *node;

    __slist_iterator_base(__slist_node_base *x) : node(x) {}

    void incr() { node = node->next; } // 向前一个节点
    bool operator==(const __slist_iterator_base &x) const { return node == x.node; }
    bool operator!=(const __slist_iterator_base &x) const { return node != x.node; }
  };
  template <class T, class Ref, class Ptr>
  struct __slist_iterator : public __slist_iterator_base
  {
    typedef __slist_iterator<T, T &, T *> iterator;
    typedef __slist_iterator<T, const T &, const T *> const_iterator;
    typedef __slist_iterator<T, Ref, Ptr> self;

    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __slist_node<T> list_node;

    __slist_iterator(list_node *x) : __slist_iterator_base(x) {}
    // 调用slist<T>::end()时会造成__slist_iterator(nullptr)，于是调用上述函数
    __slist_iterator() : __slist_iterator_base(nullptr) {}
    __slist_iterator(const iterator &x) : __slist_iterator_base(x.node) {}

    reference operator*() const { return static_cast<list_node *>(node)->data; } // ((list_node *)node)首先将node指针强制转换为list_node类型的指针
    pointer operator->() const { return &(operator*()); }                        // iterator->member通常用来访问迭代器当前指向的元素的成员
    self &operator++()
    {
      incr();       // 向前一个节点
      return *this; // this 是一个指向当前对象的指针，*this 就是解引用当前对象的指针，得到当前对象的引用
    }
    self &operator++(int)
    {
      self tmp = *this;
      incr(); // 向前一个节点
      return tmp;
    }
    // slist单向链表没有opertator--
  };

  template <class T /*,class Alloc=alloc*/>
  class slist
  {
  protected:
    typedef __slist_node<T> list_node;
    typedef __slist_node_base list_node_base;
    typedef __slist_iterator_base iterator_base;
    typedef zfwstl::simple_allocator<list_node /* , Alloc */> list_node_allocator; // 专属之空间配置器，每次分配一个节点大小
  public:
    //  专属空间配置器
    typedef zfwstl::simple_allocator<T> data_allocator;
    typedef zfwstl::simple_allocator<T> allocator_type;
    // list 的嵌套型别定义
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef list_node *link_type;

    typedef __slist_iterator<T, T &, T *> iterator;
    typedef __slist_iterator<T, const T &, const T *> const_iterator;

    allocator_type get_allocator() { return data_allocator(); }
    template <class U>
    friend void swap(slist<U> &lhs, slist<U> &rhs) noexcept;

  private:
    static list_node *create_node(const value_type &x)
    {
      list_node *node = list_node_allocator::allocate();
      try
      {
        zfwstl::construct(&node->data, x);
        node->next = nullptr;
      }
      catch (...)
      {
        list_node_allocator::deallocate(node);
      }
      return node;
    }
    static void destroy_node(list_node *node)
    {
      zfwstl::destroy(&node->data);
      list_node_allocator::deallocate(node);
    }

    list_node_base head; // 头部，不是指针！是实物
  public:
    slist() { head.next = nullptr; }
    ~slist()
    {
      clear(); // 清除所有元素
      // 不需要显式销毁成员，因为它们会自动被销毁
    }

    iterator begin() { return iterator(static_cast<list_node *>(head.next)); }
    const_iterator begin() const { return iterator(static_cast<list_node *>(head.next)); }
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator end() const { return iterator(nullptr); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    // 容量相关操作
    bool empty() const noexcept { return head.next == nullptr; }
    size_type size() const noexcept { return __slist_size(head.next); }

    // 修改容器相关操作
    void swap(slist &L)
    {
      list_node_base *tmp = head.next;
      head.next = L.head.next;
      L.head.next = tmp;
    }
    reference front() { return static_cast<list_node *>(head.next)->data; }
    void push_front(const value_type &x) { __slist_make_link(&head, create_node(x)); }
    void pop_front()
    {
      list_node *node = static_cast<list_node *>(head.next);
      head.next = node->next;
      destroy_node(node);
    }
    void clear()
    {
      while (head.next != nullptr)
      {
        pop_front();
      }
    }
  };
  //========================模板类外重载操作===============
  // 重载 zfwstl 的 swap
  template <class U>
  void swap(slist<U> &lhs, slist<U> &rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif // !ZFWSTL_SLIST_H_