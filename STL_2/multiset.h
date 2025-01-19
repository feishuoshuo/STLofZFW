#ifndef ZFWSTL_MULTISET_H_
#define ZFWSTL_MULTISET_H_
/**
 * key值可重复的set
 */
#include <cstddef>             //for size_t, ptrdiff_t
#include "../src/functional.h" //for less<>, identity
#include "rb_tree.h"
#include "../src/util.h" //for  pair<iterator, bool>
namespace zfwstl
{

  template <class Key, class Compare = zfwstl::less<Key> /* ,class Alloc=alloc */>
  class multiset
  {
  public:
    typedef Key key_type;
    typedef Key value_type;
    // 注意，key与value使用同一个比较函数
    typedef Compare key_compare;
    typedef Compare value_compare;

  private:
    typedef rb_tree<key_type, value_type, zfwstl::identity<value_type>, key_compare> rep_type;
    rep_type t; // 采用红黑树表现multiset

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

    multiset() : t(Compare()) {}
    explicit multiset(const Compare &comp) : t(comp) {}
    // multiset允许相同键值存在->insert_equal
    template <class InputIter>
    multiset(InputIter first, InputIter last) : t(Compare()) { t.insert_equal(first, last); }
    template <class InputIter>
    multiset(InputIter first, InputIter last, const Compare &comp) : t(comp) { t.insert_equal(first, last); }
    multiset(std::initializer_list<value_type> ilist)
        : t()
    {
      t.insert_equal(ilist.begin(), ilist.end());
    }

    multiset(const multiset &x) : t(x.t) {}
    multiset(multiset &&x) : t(zfwstl::move(x.t)) {}
    multiset &operator=(const multiset &x)
    {
      t = x.t;
      return *this;
    }
    multiset &operator=(multiset &&rhs)
    {
      t = zfwstl::move(rhs.t);
      return *this;
    }
    multiset &operator=(std::initializer_list<value_type> ilist)
    {
      t.clear();
      t.insert_equal(ilist.begin(), ilist.end());
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
    void swap(multiset<Key, Compare> &x) { t.swap(x.t); }
    //==================插入删除操作==================
    void clear() { t.clear(); }

    typedef std::pair<iterator, bool> pair_iterator_bool;

    iterator insert(const value_type &x)
    {
      auto p = t.insert_equal(x);
      return p;
    }
    // iterator insert(iterator position, const value_type &x)
    // {
    //   typedef typename rep_type::iterator rep_iterator;
    //   return t.insert_equal((rep_iterator &)position, x);
    // }
    template <class InputIter>
    void insert(InputIter first, InputIter last)
    {
      t.insert_equal(first, last);
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
    size_type count(const key_type &x) const { return t.count_multi(x); }
    iterator lower_bound(const key_type &x) const { return t.lower_bound(x); }
    iterator upper_bound(const key_type &x) const { return t.upper_bound(x); }
    zfwstl::pair<iterator, iterator>
    equal_range(const key_type &x) const
    {
      return t.equal_range(x);
    }

  public:
    friend bool operator==(const multiset &lhs, const multiset &rhs) { return lhs.t == rhs.t; }
    friend bool operator<(const multiset &lhs, const multiset &rhs) { return lhs.t < rhs.t; }
  };
  // 重载比较操作符
  template <class Key, class Compare>
  bool operator==(const multiset<Key, Compare> &lhs, const multiset<Key, Compare> &rhs)
  {
    return lhs.t == rhs.t;
  }
  template <class Key, class Compare>
  bool operator<(const multiset<Key, Compare> &lhs, const multiset<Key, Compare> &rhs)
  {
    return lhs.t < rhs.t;
  }
  template <class Key, class Compare>
  bool operator!=(const multiset<Key, Compare> &lhs, const multiset<Key, Compare> &rhs)
  {
    return !(lhs == rhs);
  }
  template <class Key, class Compare>
  bool operator>(const multiset<Key, Compare> &lhs, const multiset<Key, Compare> &rhs)
  {
    return rhs < lhs;
  }
  template <class Key, class Compare>
  bool operator<=(const multiset<Key, Compare> &lhs, const multiset<Key, Compare> &rhs)
  {
    return !(rhs < lhs);
  }
  template <class Key, class Compare>
  bool operator>=(const multiset<Key, Compare> &lhs, const multiset<Key, Compare> &rhs)
  {
    return !(lhs < rhs);
  }

}

#endif // !ZFWSTL_MULTISET_H_