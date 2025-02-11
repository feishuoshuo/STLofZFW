#ifndef ZFWSTL_SET_H_
#define ZFWSTL_SET_H_
/**
 * set集合: key即value
 * 底层迭代器：const iterator，也就是不能修改集合中插入的元素
 */
#include <cstddef>             //for size_t, ptrdiff_t
#include "../src/functional.h" //for less<>, identity
#include "rb_tree.h"
#include "../src/util.h" //for  pair<iterator, bool>, move()
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

  private:
    typedef rb_tree<key_type, value_type, zfwstl::identity<value_type>, key_compare> rep_type;
    rep_type t; // 采用红黑树表现set

  public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_iterator iterator; //!!底层迭代器const_iterator
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::const_reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    set() : t(Compare()) {}
    explicit set(const Compare &comp) : t(comp) {}
    set(std::initializer_list<value_type> ilist)
    {
      t.insert_unique(ilist.begin(), ilist.end());
    }
    // set不允许相同键值存在->insert_unique
    template <class InputIter>
    set(InputIter first, InputIter last) : t(Compare()) { t.insert_unique(first, last); }
    template <class InputIter>
    set(InputIter first, InputIter last, const Compare &comp) : t(comp) { t.insert_unique(first, last); }
    set(const set &other) : t(other.t) {}
    set(set &&other) : t(zfwstl::move(other.t)) {}
    template <typename K2, typename Compare2>
    set(const set<K2, Compare2> &other) : t(other.t) {} // 新增的模板构造函数
    template <typename K2, typename Compare2>
    set(const set<K2, Compare2> &&x) : t(zfwstl::move(x.t)) {}

    set &operator=(const set &x)
    {
      t = x.t;
      return *this;
    }
    set &operator=(set &&x)
    {
      t = zfwstl::move(x.t);
      return *this;
    }
    set &operator=(std::initializer_list<value_type> ilist)
    {
      t.clear();
      t.insert_unique(ilist.begin(), ilist.end());
      return *this;
    }

  public:
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return t.key_comp(); }
    const_iterator begin() const { return t.begin(); }
    const_iterator end() const { return t.end(); }
    iterator begin() { return t.begin(); }
    iterator end() { return t.end(); }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }
    bool empty() { return t.empty(); }
    size_type size() const noexcept { return t.size(); }
    size_type max_size() const noexcept { return t.max_size(); }
    void swap(set<Key, Compare> &x) { t.swap(x.t); }
    //==================插入删除操作==================
    void clear() { t.clear(); }

    typedef zfwstl::pair<iterator, bool> pair_iterator_bool;

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
    template <class InputIter>
    void insert(InputIter first, InputIter last)
    {
      t.insert_unique(first, last);
    }
    void erase(iterator position)
    {
      typedef typename rep_type::iterator rep_iterator;
      t.erase(static_cast<rep_iterator>(position));
    }
    size_type erase(const key_type &x)
    {
      return t.erase(x);
    }
    void erase(iterator first, iterator last) { t.erase(first, last); }

    iterator find(const key_type &x) const { return t.find(x); }
    size_type count(const key_type &x) const { return t.count_unique(x); }
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
    return !(lhs == rhs);
  }
  template <class Key, class Compare>
  bool operator>(const set<Key, Compare> &lhs, const set<Key, Compare> &rhs)
  {
    return rhs < lhs;
  }
  template <class Key, class Compare>
  bool operator<=(const set<Key, Compare> &lhs, const set<Key, Compare> &rhs)
  {
    return !(rhs < lhs);
  }
  template <class Key, class Compare>
  bool operator>=(const set<Key, Compare> &lhs, const set<Key, Compare> &rhs)
  {
    return !(lhs < rhs);
  }
}

#endif // !ZFWSTL_SET_H_