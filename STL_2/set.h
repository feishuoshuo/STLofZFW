#ifndef ZFWSTL_SLIST_H_
#define ZFWSTL_SLIST_H_
/**
 * set集合: key即value
 * 底层迭代器：const iterator，也就是不能修改集合中插入的元素
 */
#include <cstddef>             //for size_t, ptrdiff_t
#include "../src/functional.h" //for less<>, identity
#include "rb_tree.h"
#include "../src/util.h" //for  pair<iterator, bool>
namespace zfwstl
{

  template <class Key, class Compare = zfwstl::less<Key> /* ,class Alloc=alloc */>
  class set
  {
  public:
    typedef Key key_type;
    typedef Key value_type;
    // 注意，key与value使用同一个比较函数
    typedef Compare key_compare;
    typedef Compare value_compare;
    typedef rb_tree<key_type, value_type, zfwstl::identity<value_type>, key_compare> rep_type;
    rep_type t; // 采用红黑树表现set

  public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_iterator iterator; //!!底层迭代器const_iterator
    typedef typename rep_type::const_iterator const_iterator;
    // TODO：反转迭代器之后补上
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    set() : t(Compare()) {}
    explicit set(const Compare &comp) : t(comp) {}
    // set不允许相同键值存在->insert_unique
    template <class InputIter>
    set(InputIter first, InputIter last) : t(Compare()) { t.insert_unique(first, last); }
    template <class InputIter>
    set(InputIter first, InputIter last, const Compare &comp) : t(comp) { t.insert_unique(first, last); }
    set(const set<Key, Compare /* ,Alloc */> &x) : t(x.t) {}

    set<Key, Compare /* ,Alloc */> &operator=(const set<Key, Compare /* ,Alloc */> &x)
    {
      t = x.t;
      return *this;
    }

  public:
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return t.key_comp(); }
    iterator begin() const { return t.begin(); }
    iterator end() const { return t.end(); }
    iterator rend() { return t.rend(); }
    iterator rbegin() { return t.rbegin(); }
    bool empty() { return t.empty(); }
    size_type size() const noexcept { return t.size(); }
    size_type max_size() const noexcept { return t.max_size(); }
    void swap(set<Key, Compare> &x) { t.swap(x.t); }
    //==================插入删除操作==================
    void clear() { t.clear(); }

    typedef std::pair<iterator, bool> pair_iterator_bool;

    pair_iterator_bool insert(const value_type &x)
    {
      auto p = t.insert_unique(x);
      return p;
    }
    // iterator insert(iterator position, const value_type &x)
    // {
    //   typedef typename rep_type::iterator rep_iterator;
    //   return t.insert_unique((rep_iterator &)position, x);
    // }
    // typename<class InputIter> void insert(InputIter first, InputIter last)
    // {
    //   t.insert_unique(first, last);
    // }
    void erase(iterator position)
    {
      typedef typename rep_type::iterator rep_iterator;
      t.erase(static_cast<rep_iterator>(position));
    }
    size_type erase(const key_type &x)
    {
      return t.erase(x);
    }

    iterator find(const key_type &x) const { return t.find(x); }
    size_type count(const key_type &x) const { return t.count(x); }
    iterator lower_bound(const key_type &x) const { return t.lower_bound(x); }
    iterator upper_bound(const key_type &x) const { return t.upper_bound(x); }
    zfwstl::pair<iterator, iterator>
    equal_range(const key_type &x) const
    {
      return t.equal_range(x);
    }

  public:
    friend bool operator==(const set &lhs, const set &rhs) { return lhs.t == rhs.t; }
    friend bool operator<(const set &lhs, const set &rhs) { return lhs.t < rhs.t; }
  };
  // 重载比较操作符
  template <class Key, class Compare>
  bool operator==(const set<Key, Compare> &lhs, const set<Key, Compare> &rhs)
  {
    return lhs.t == rhs.t;
  }
  template <class Key, class Compare>
  bool operator<(const set<Key, Compare> &lhs, const set<Key, Compare> &rhs)
  {
    return lhs.t < rhs.t;
  }
  template <class Key, class Compare>
  bool operator!=(const set<Key, Compare> &lhs, const set<Key, Compare> &rhs)
  {
    return !(lhs.t == rhs.t);
  }
  template <class Key, class Compare>
  bool operator>(const set<Key, Compare> &lhs, const set<Key, Compare> &rhs)
  {
    return rhs.t < lhs.t;
  }
  template <class Key, class Compare>
  bool operator<=(const set<Key, Compare> &lhs, const set<Key, Compare> &rhs)
  {
    return !(rhs.t < lhs.t);
  }
  template <class Key, class Compare>
  bool operator>=(const set<Key, Compare> &lhs, const set<Key, Compare> &rhs)
  {
    return !(lhs.t < rhs.t);
  }
}

#endif // !ZFWSTL_SLIST_H_