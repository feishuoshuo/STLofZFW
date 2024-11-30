#ifndef ZFWSTL_LIST_H_
#define ZFWSTL_LIST_H_
/**
 * 环状双向链表(circle double-link-list)
 * Bidirectional iterators
 */
#include <cstddef>                   //for size_t, ptrdiff_t
#include "../src/memory/allocator.h" //标准空间配置器
#include "../src/iterator.h"         // for bidirectional_iterator_tag
#include "../src/exceptdef.h"        //for MYSTL_DEBUG, MYSTL_DEBUG
#include "../src/functional.h"       //函数对象 less<T>()
#include "../src/util.h"             //for swap
namespace zfwstl
{
  // ===========================list的节点node结构=============================
  template <class T>
  struct __list_node
  {
    /**
     * void*，这样做的原因是为了允许链表存储不同类型的数据，
     * 同时避免直接使用具体类型指针（如 __list_node<T>*）可能带来的类型安全问题
     */
    typedef void *void_pointer;
    void_pointer prev; // 型别 void*,其实可设为__list_node<T>*
    void_pointer next;
    T data;
  };

  template <class T, class Ref, class Ptr>
  struct __list_iterator
  {
    typedef __list_iterator<T, T &, T *> iterator; // 指向值类型 T 的迭代器:通用迭代器
    typedef __list_iterator<T, const T &, const T *> const_iterator;
    typedef __list_iterator<T, Ref, Ptr> self; // 当前迭代器的类型:自引用迭代器

    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    /**
     * vector 的迭代器直接使用 T& 作为引用类型，这是因为 vector 存储的是实际的对象，并且这些对象是连续存储的。
     * 因此，迭代器可以直接返回对象的引用。
     *
     * 然而，对于 list（或任何非连续存储的容器，如 set、map 等），情况有所不同，
     * 因为它们通常基于节点和指针来管理元素，而不是直接存储对象.
     * 使用 Ref 作为 reference 类型允许迭代器返回一个引用或代理，而不是对象的实际引用
     */
    typedef Ref reference;
    typedef __list_node<T> *link_type; // 链表节点的指针类型
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    link_type node; // 迭代器内部普通指针，指向list节点

    // constructor
    __list_iterator(link_type x) : node(x) {}
    __list_iterator() {}
    __list_iterator(const iterator &x) : node(x.node) {}

    /**
     * 检查当前迭代器（*this）是否与另一个迭代器（x）相等
     * 用于比较两个迭代器对象是否指向链表中的同一个节点
     */
    bool operator==(const self &x) const { return node == x.node; }
    bool operator!=(const self &x) const { return node != x.node; }
    // TODO：没搞懂
    //  对迭代器取值(dereference)，取的是节点的数据值
    reference operator*() const { return (*node).data; } //(*node)先解引用得到node指针指向的节点对象，然后.data再访问数据值
    // 对迭代器的成员存取(member access)运算子的标准做法
    pointer operator->() const { return &(operator*()); }
    // 前进一个节点
    self &operator++()
    {
      MYSTL_DEBUG(node != nullptr);
      node = static_cast<link_type>((*node).next);
      /**
       * 如果operator++()不返回*this的引用，
       * 那么每次循环都会创建一个新的迭代器对象，
       * 这将导致不必要的性能开销
       */
      // TODO:??
      return *this;
    }
    self &operator++(int)
    { // 后置++
      self tmp = *this;
      ++*this; //(*this)解引用得到指针(self自迭代器)所指当前对象，然后调用operator++
      return tmp;
    }
    self &operator--()
    {
      MYSTL_DEBUG(node != nullptr);
      node = static_cast<link_type>((*node).prev);
      return *this;
    }
    self &operator--(int)
    {
      self tmp = *this;
      --*this;
      return tmp;
    }
  };

  //=========================list=========================
  template <class T /*,class Alloc=alloc*/>
  class list
  {
  protected:
    typedef __list_node<T> list_node;
    typedef zfwstl::simple_allocator<list_node /* , Alloc */> list_node_allocator; // 专属之空间配置器，每次分配一个节点大小
  public:
    //  标准空间配置器
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

    typedef __list_iterator<T, T &, T *> iterator;
    typedef __list_iterator<const T, const T &, const T *> const_iterator;
    // TODO:以后可以添加反向迭代器
    // typedef mystl::reverse_iterator<iterator> reverse_iterator;
    // typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

    allocator_type get_allocator() { return data_allocator(); }
    template <class U>
    friend void swap(list<U> &lhs, list<U> &rhs) noexcept;

  private:
    link_type node;  // 只要一个指针，便可表示整个环状双向链表 [就是空白节点]
    size_type size_; // 大小，不算里面那个空白节点

  public:
    // 构造、复制、移动、析构函数
    list() { empty_initialize(0, value_type()); } // 产生一个空链表 Tip:当中存在一个空白节点
    explicit list(size_type n)
    {
      empty_initialize(n, value_type());
    }

    //=================operator操作运算符重载=====================

    // 迭代器相关操作
    iterator begin() noexcept
    {
      return iterator(static_cast<__list_node<T> *>(node->next));
    } // 等价于 static_cast<link_type>((*node).next)
    const_iterator begin() const noexcept { return iterator(static_cast<__list_node<T> *>(node->next)); }
    iterator end() noexcept { return node; }
    const_iterator end() const noexcept { return node; }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }
    // 容量相关操作
    bool empty() const noexcept { return node->next == node; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); } // 理论最大大小

    // 访问元素相关操作
    reference front()
    {
      MYSTL_DEBUG(!empty());
      return *begin();
    }
    const_reference front() const
    {
      MYSTL_DEBUG(!empty());
      return *begin();
    }
    reference back()
    {
      MYSTL_DEBUG(!empty());
      return *(--end());
    }
    const_reference back() const
    {
      MYSTL_DEBUG(!empty());
      return *(--end());
    }

    // 修改容器相关操作
    // TODO:修改容器相关操作
    void assign();
    void emplace_front();
    void emplace_back();
    void emplace();
    // 传入一个 new_size，如果链表旧长度大于 new_size 的大小, 那就删除后面多余的节点
    void resize();
    iterator insert(iterator position, const T &x)
    {
      // 头插法
      link_type tmp = create_node(x); // 创造一个节点
      // 插入链表的具体位置中
      tmp->next = position.node;
      tmp->prev = position.node->prev;
      static_cast<link_type>(position.node->prev)->next = tmp; // 防止该链表就一个节点
      position.node->prev = tmp;
      ++size_; // 链表个数 +1
      return tmp;
    }
    void push_front(const T &x) { insert(begin(), x); }
    void push_back(const T &x) { insert(end(), x); }
    void pop_front() { erase(begin()); }
    void pop_back()
    {
      iterator tmp = end();
      erase(--tmp);
    }
    iterator erase(iterator position)
    {
      MYSTL_DEBUG(size_ <= 0);
      link_type next_node = static_cast<link_type>(position.node->next);
      link_type prev_node = static_cast<link_type>(position.node->prev);
      prev_node->next = next_node;
      next_node->prev = prev_node;
      destroy_node(position.node);
      --size_; // 链表个数 -1
      return iterator(next_node);
    }
    void remove(const T &value)
    {
      iterator first = begin();
      iterator last = end();
      while (first != last)
      {
        iterator next = first;
        ++next;
        if (*first == value)
          erase(first);
        first = next;
      }
    }
    // 移除数值相同的连续元素：只有“连续而相同的元素”，才会被移除剩下一个
    void unique()
    {
      iterator first = begin();
      iterator last = end();
      if (first == last)
        return; // 空链表
      iterator next = first;
      while (++next != last)
      {
        if (*first == *next)
          erase(next);
        else
          first = next;
        next = first; // 修正区段范围
      }
    }
    void clear()
    {
      if (size_ == 0)
        return;
      link_type cur = static_cast<link_type>(node->next);
      while (cur != node)
      {
        link_type tmp = cur;
        cur = static_cast<link_type>(cur->next);
        destroy_node(tmp);
      }
      // 恢复 node 原始状态
      node->next = node;
      node->prev = node;
      size_ = 0;
    }
    // 交换两个list容器的内容
    void swap(list &rhs) noexcept
    {
      zfwstl::swap(node, rhs.node);
      zfwstl::swap(size_, rhs.size_);
    }
    // 原地交换两个节点node的data值
    void swap(iterator p, iterator q)
    {
      auto tmp = *p;
      *p = *q;
      *q = tmp;
    }
    // 将 x接合于 position所指位置之前。x必须不同于 *this。
    void splice(iterator position, list &__x)
    {
      if (!__x.empty())
        this->transfer(position, __x.begin(), __x.end());
    }
    // 将 i所指元素接合于 position所指位置之前。position和 i可指向同一个 list
    void splice(iterator position, list &, iterator i)
    {
      iterator j = i;
      ++j;
      if (position == i || position == j)
        return;
      this->transfer(position, i, j);
    }
    // 将 [first,last)内的所有元素接合于 position所指位置之前。
    // position和[first,last)可指向同一个 list，
    // 但 position不能位于[first,last)之内。
    void splice(iterator position, list &, iterator first, iterator last)
    {
      if (first != last)
        this->transfer(position, first, last);
    }
    // list 相关操作 remove remove_if merge sort reverse
    // 将*this内容逆向重置
    void reverse()
    {
      // size_==0||size_==1 也可以但是比这个慢
      if (node->next == node || static_cast<link_type>(node->next)->next == node)
        return;
      iterator first = begin();
      ++first;
      while (first != end())
      {
        iterator old = first;
        ++first;
        transfer(begin(), old, first);
      }
    }
    // 因为STL算法sort()只接受RamdonAccessIterator
    // 以下sort可选择排序手段
    void sort(const char &x = '\0') // 默认值是空字符
    {
      if (node->next == node || static_cast<link_type>(node->next)->next == node)
        return;
      MYSTL_DEBUG(x != 'q' || x != 'm' || x != '\0');
      if (x == '\0')
        _sort(zfwstl::less<T>()); // SGI版本
      else if (x == 'q')
        quick_sort(begin(), end(), zfwstl::less<T>());
      else
        merge_sort(begin(), end(), size_, zfwstl::less<T>());
    }
    template <class Compared>
    void sort(Compared comp, const char &x = '\0') // 默认值是空字符
    {
      if (node->next == node || static_cast<link_type>(node->next)->next == node)
        return;
      MYSTL_DEBUG(x != 'q' || x != 'm' || x != '\0');
      if (x == '\0')
        _sort(comp); // SGI版本
      else if (x == 'q')
        quick_sort(begin(), end(), comp);
      else
        merge_sort(comp);
    }
    void merge(list &x) { _merge(x, zfwstl::less<T>()); }
    template <class Compared>
    void merge(list &x, Compared comp) { _merge(x, comp); }

  protected:
    // ================分别是 配置、释放、构造、销毁一个节点===================
    link_type get_node()
    {
      return list_node_allocator::allocate();
    }
    void put_node(link_type p) { list_node_allocator::deallocate(p); }
    // 产生(配置并构造)一个节点，带元素值
    link_type create_node(const T &x)
    {
      link_type p = get_node();
      zfwstl::construct(&p->data, x); //(&(p->data))取p指向的data成员的地址
      return p;
    }
    // 销毁(析构并释放)一个节点
    void destroy_node(link_type p)
    {
      zfwstl::destroy(&p->data);
      put_node(p);
    }
    void empty_initialize(size_type n, const value_type &value)
    {
      node = get_node(); // 配置一个节点空间，令 node 指向它
      size_ = 0;
      if (n == 0)
      {
        node->next = node;
        node->prev = node;
      }
      else
      {
        try
        {
          for (; n > 0; --n)
            insert(begin(), value);
        }
        catch (...)
        {
          clear();
          throw;
        }
      }
    }
    // 元素搬移 [first, last)搬移到position之前
    /**
     * 将某连续范围的元素从一个list移动到另一个(或同一个)list的某个定点
     */
    void transfer(iterator position, iterator first, iterator last)
    {
      if (position == last)
        return;
      // Remove [first, last) from its old position.
      static_cast<link_type>(last.node->prev)->next = position.node;
      static_cast<link_type>(first.node->prev)->next = last.node;
      static_cast<link_type>(position.node->prev)->next = first.node;

      // Splice [first, last) into its new position.
      auto tmp = position.node->prev;
      position.node->prev = last.node->prev;
      last.node->prev = first.node->prev;
      first.node->prev = tmp;
    }
    // merge()将 x 合并到 *this 身上。两个 lists 的内容都必须先经过递增/递减排序
    template <class Compared>
    void _merge(list &x, Compared comp)
    {
      iterator first1 = begin();
      iterator last1 = end();
      iterator first2 = x.begin();
      iterator last2 = x.end();
      // 前提：两个 lists 都已经过递增/递减排序
      while (first1 != last1 && first2 != last2)
      {
        if (comp(*first2, *first1)) //*first2 < *first1
        {
          iterator next = first2;
          transfer(first1, first2, ++next);
          first2 = next;
        }
        else
          ++first1;
      }
      if (first2 != last2)
        transfer(last1, first2, last2);
    }
    template <class Compared>
    void _sort(Compared comp)
    {
      // NOTE:“基数排序”（Radix Sort）的变种
      /**
       * 它避免了随机访问，而链表的随机访问效率较低。
       * 通过使用链表的 splice 和 swap 操作，算法能够有效地对链表进行排序
       */
      list<T> carry;       // 临时链表，用于存储从主链表中取出的元素
      list<T> counter[64]; // 一个包含64个链表的数组 counter，用于基数排序中的桶（bucket）
      int fill = 0;        // 一个计数器, 用于记录 counter 数组中有多少个链表非空
      while (!empty())
      {
        carry.splice(carry.begin(), *this, begin()); // 将主链表的第一个元素移动到 carry 链表中
        int i = 0;
        while (i < fill && !counter[i].empty())
        {
          counter[i]._merge(carry, comp);
          carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill)
          ++fill;
      }
      for (int i = 1; i < fill; ++i)
        counter[i]._merge(counter[i - 1], comp);
      swap(counter[fill - 1]); // 将 counter[fill-1] 中的元素合并回主链表
    }
    // 快排[不稳定]
    template <class Compared>
    void quick_sort(iterator _left, iterator _right, Compared comp)
    {
      // tmp为基准数(哨兵)
      iterator tmp = _left;
      --_right;
      // 确定边界区域
      iterator last = _right;
      iterator first = _left;
      while (_left != _right)
      {
        if (_right != tmp)
        {
          if (comp(*_right, *tmp)) //*_right < *tmp
          {
            swap(_left, _right);
            ++_left;
            tmp = _right;
          }
          else
            --_right;
        }
        else
        {
          if (comp(*_left, *tmp)) //*_left < *tmp
            ++_left;
          else
          {
            swap(_left, _right);
            --_right;
            tmp = _left;
          }
        }
      }
      if (first != tmp)
        quick_sort(first, tmp, comp);
      if (last != tmp)
        quick_sort(++tmp, ++last, comp);
    }
    // 归并排序[_left1, _right2)
    template <class Compared>
    iterator merge_sort(iterator _left1, iterator _right2, size_type n, Compared comp)
    {
      --_right2;
      if (_left1 == _right2)
        return _left1;
      auto n2 = n / 2;
      auto _right1 = _left1;
      zfwstl::advance(_right1, n2);
      auto result = _left1 = merge_sort(_left1, _right1, n2, comp); // 前半段的最小位置
      auto _left2 = merge_sort(_right1, ++_right2, n - n2, comp);   // 后半段的最小位置
      if (comp(*_left2, *_left1))                                   //*_left1 > *_left2
        result = _left2;                                            // result优先指向两端中开头最小值的节点
      // 合并两段有序区间
      while (_left1 != _right1 && _left2 != _right2)
      {
        if (comp(*_left2, *_left1)) //*_left1 > *_left2
        {
          auto next = _left2;
          transfer(_left1, _left2, ++next);
          _left2 = next;
        }
        else
          ++_left1;
      }
      return result;
    }
  };

  //========================模板类外重载操作===============
  // TODO:operator操作运算符重载
  // 重载 zfwstl 的 swap
  template <class U>
  void swap(list<U> &lhs, list<U> &rhs) noexcept
  {
    lhs.swap(rhs);
  }
}
#endif // !ZFWSTL_LIST_H_