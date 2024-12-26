#ifndef ZFWSTL_UNORDERED_SET_H_
#define ZFWSTL_UNORDERED_SET_H_
/**
 * unordered_set
 */
#include <cstddef> //for size_t, ptrdiff_t
#include "hashtable.h"
#include "../src/functional.h" //for identity
#include "../src/util.h"       //for pair
namespace zfwstl
{

  template <class Value, class HashFcn, class EqualKey>
  class unordered_set
  {
    typedef hashtable<Value, Value, HashFcn, zfwstl::identity<Value>, EqualKey> ht;
    ht rep; // 底层机制hash table完成
  public:
    typedef typename ht::key_type key_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;
    typedef typename ht::const_pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::const_reference reference;
    typedef typename ht::const_reference const_reference;
    typedef typename ht::const_iterator iterator; //!!底层迭代器const_iterator
    typedef typename ht::const_iterator const_iterator;
    // TODO：反转迭代器之后补上
    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;

  public:
    // 缺省使用大小=100的表格。将被hash table调整为最接近且较大之质数
    unordered_set() : rep(100, hasher(), key_equal()) {}
    explicit unordered_set(size_type n) : rep(n, hasher(), key_equal()) {}
    unordered_set(size_type n, const hasher &hf) : rep(n, hf, key_equal()) {}
    unordered_set(size_type n, const hasher &hf, const key_equal &eql) : rep(n, hf, eql) {}

    template <class InputIter>
    unordered_set(InputIter f, InputIter l) : rep(100, hasher(), key_equal()) { rep.insert_unique(f, l); }
    template <class InputIter>
    unordered_set(InputIter f, InputIter l, size_type n) : rep(n, hasher(), key_equal()) { rep.insert_unique(f, l); }
    template <class InputIter>
    unordered_set(InputIter f, InputIter l, size_type n, const hasher &hf) : rep(n, hf, key_equal()) { rep.insert_unique(f, l); }
    template <class InputIter>
    unordered_set(InputIter f, InputIter l, size_type n, const hasher &hf, const key_equal &eql) : rep(n, hf, eql) { rep.insert_unique(f, l); }

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }
    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }
    size_type size() const { return rep.size(); }
    bool empty() { return rep.empty(); }
    size_type max_size() const noexcept { return rep.max_size(); }
    void swap(unordered_set &x) { rep.swap(x.rep); }

    // 用于比较两个 unordered_set 对象是否相等和不等的运算符重载函数
    friend bool operator==(const unordered_set &lhs, const unordered_set &rhs)
    {
      return lhs.rep.equal_range_unique(rhs.rep);
    }
    friend bool operator!=(const unordered_set &lhs, const unordered_set &rhs)
    {
      return !lhs.rep.equal_range_unique(rhs.rep);
    }

    zfwstl::pair<iterator, bool> insert(const value_type &obj)
    {
      auto p = rep.insert_unique(obj);
      return zfwstl::pair<iterator, bool>(p.first, p.second);
    }
    template <class InputIter>
    void insert(InputIter f, InputIter l) { rep.insert_unique(f, l); }
    zfwstl::pair<iterator, bool> insert_noresize(const value_type &obj)
    {
      zfwstl::pair<typename ht::iterator, bool> p = rep.insert_unique_noresize(obj);
      return zfwstl::pair<iterator, bool>(p.first, p.second);
    }
    iterator find(const key_type &key) const { return rep.find(key); }
    size_type count(const key_type &key) const { return rep.count(key); }
    zfwstl::pair<iterator, iterator> equal_range(const key_type &key) const { return rep.equal_range_unique(key); }
    size_type erase(const key_type &key) { return rep.erase(key); }
    void erase(iterator it) { rep.erase(it); }
    void clear() { rep.clear(); }

  public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }
  };
  template <class Value, class HashFcn, class EqualKey>
  inline bool operator==(const unordered_set<Value, HashFcn, EqualKey> &lhs,
                         const unordered_set<Value, HashFcn, EqualKey> &rhs)
  {
    return lhs.rep == rhs.rep;
  }

  template <class Value, class HashFcn, class EqualKey>
  inline bool operator!=(const unordered_set<Value, HashFcn, EqualKey> &lhs,
                         const unordered_set<Value, HashFcn, EqualKey> &rhs)
  {
    return lhs.rep != rhs.rep;
  }

}

#endif // !ZFWSTL_UNORDERED_SET_H_