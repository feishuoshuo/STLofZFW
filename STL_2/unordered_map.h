#ifndef ZFWSTL_UNORDERED_MAP_H_
#define ZFWSTL_UNORDERED_MAP_H_
/**
 * unordered_map
 */
#include <cstddef> //for size_t, ptrdiff_t
#include "hashtable.h"
#include "../src/functional.h"           //for select1st, equal_to, hash
#include "../src/util.h"                 //for pair, move
#include "../src/algorithms/algorithm.h" //for max
namespace zfwstl
{

  template <class Key, class Tp, class HashFcn = zfwstl::hash<Key>, class EqualKey = zfwstl::equal_to<Key>>
  class unordered_map
  {
    typedef hashtable<zfwstl::pair<const Key, Tp>, Key, HashFcn, zfwstl::select1st<zfwstl::pair<const Key, Tp>>, EqualKey> ht;
    ht rep; // 底层机制hash table完成
  public:
    typedef typename ht::key_type key_type;
    typedef typename ht::value_type value_type;
    typedef Tp data_type;
    typedef Tp mapped_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;
    typedef typename ht::pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::reference reference;
    typedef typename ht::const_reference const_reference;
    typedef typename ht::iterator iterator;
    typedef typename ht::const_iterator const_iterator;
    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;

  public:
    // 缺省使用大小=100的表格。将被hash table调整为最接近且较大之质数
    unordered_map() : rep(100, hasher(), key_equal()) {}
    explicit unordered_map(size_type n) : rep(n, hasher(), key_equal()) {}
    unordered_map(size_type n, const hasher &hf) : rep(n, hf, key_equal()) {}
    unordered_map(size_type n, const hasher &hf, const key_equal &eql) : rep(n, hf, eql) {}
    unordered_map(std::initializer_list<value_type> ilist,
                  const hasher &hf = zfwstl::hash<key_type>(),
                  const key_equal &equal = zfwstl::equal_to<key_type>())
        : rep(zfwstl::max<size_type>(100, static_cast<size_type>(ilist.size())), hf, equal)
    {
      rep.insert_equal(ilist.begin(), ilist.end());
    }
    template <class InputIter>
    unordered_map(InputIter f, InputIter l) : rep(100, hasher(), key_equal()) { rep.insert_equal(f, l); }
    template <class InputIter>
    unordered_map(InputIter f, InputIter l, size_type n) : rep(n, hasher(), key_equal()) { rep.insert_equal(f, l); }
    template <class InputIter>
    unordered_map(InputIter f, InputIter l, size_type n, const hasher &hf) : rep(n, hf, key_equal()) { rep.insert_equal(f, l); }
    template <class InputIter>
    unordered_map(InputIter f, InputIter l, size_type n, const hasher &hf, const key_equal &eql) : rep(n, hf, eql) { rep.insert_equal(f, l); }

    unordered_map(const unordered_map &rhs) : rep(rhs.rep) {}
    unordered_map(unordered_map &&rhs) noexcept : rep(zfwstl::move(rhs.rep)) {}

    unordered_map &operator=(const unordered_map &rhs)
    {
      rep = rhs.rep;
      return *this;
    }
    unordered_map &operator=(unordered_map &&rhs)
    {
      rep = zfwstl::move(rhs.rep);
      return *this;
    }

    unordered_map &operator=(std::initializer_list<value_type> ilist)
    {
      rep.clear();
      rep.reserve(ilist.size());
      for (auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
        rep.insert_unique_noresize(*first);
      return *this;
    }

    ~unordered_map() = default;

    Tp &operator[](const key_type &key)
    {
      return rep.find_or_insert(value_type(key, Tp())).second;
    }

    Tp &operator[](key_type &&key)
    {
      return rep.find_or_insert(value_type(zfwstl::move(key), Tp())).second;
    }

    // 以下插入操作全部使用insert_equal(), 允许键值重复
    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }
    iterator begin() { return rep.begin(); }
    iterator end() { return rep.end(); }
    const_iterator begin() const { return rep.begin(); }
    const_iterator end() const { return rep.end(); }
    const_iterator cbegin() const { return rep.cbegin(); }
    const_iterator cend() const { return rep.cend(); }
    size_type size() const { return rep.size(); }
    bool empty() { return rep.empty(); }
    size_type max_size() const noexcept { return rep.max_size(); }
    void swap(unordered_map &x) { rep.swap(x.rep); }

    // 用于比较两个 unordered_map 对象是否相等和不等的运算符重载函数
    friend bool operator==(const unordered_map &lhs, const unordered_map &rhs)
    {
      // return lhs.rep.equal_range_multi(rhs.rep);
      if (lhs.size() != rhs.size())
        return false; // 如果两个unordered_map的大小不同，它们肯定不相等
      for (const auto &pair : lhs)
      {
        auto it = rhs.find(pair.first); // 在rhs中查找lhs的键
        if (it == rhs.end() || it->second != pair.second)
          return false; // 如果在rhs中找不到对应的键，或者值不相等，则返回false
      }
      return true; // 所有键值对都匹配，返回true
    }
    friend bool operator!=(const unordered_map &lhs, const unordered_map &rhs)
    {
      return !(lhs == rhs);
    }

    iterator insert(const value_type &obj)
    {
      return rep.insert_equal(obj);
    }
    template <class InputIter>
    void insert(InputIter f, InputIter l) { rep.insert_equal(f, l); }
    zfwstl::pair<iterator, bool> insert_noresize(const value_type &obj)
    {
      zfwstl::pair<typename ht::iterator, bool> p = rep.insert_equal_noresize(obj);
      return zfwstl::pair<iterator, bool>(p.first, p.second);
    }
    const_iterator find(const key_type &key) const { return rep.find(key); }
    size_type count(const key_type &key) const { return rep.count(key); }
    zfwstl::pair<iterator, iterator> equal_range(const key_type &key) const { return rep.equal_range_unique(key); }
    size_type erase(const key_type &key) { return rep.erase(key); }
    void erase(iterator it) { rep.erase(it); }
    void erase(iterator first, iterator last) { rep.erase(first, last); }
    void clear() { rep.clear(); }

  public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }
  };
  template <class Key, class Tp, class HashFcn, class EqualKey>
  inline bool operator==(const unordered_map<Key, Tp, HashFcn, EqualKey> &lhs,
                         const unordered_map<Key, Tp, HashFcn, EqualKey> &rhs)
  {
    return lhs.rep == rhs.rep;
  }

  template <class Key, class Tp, class HashFcn, class EqualKey>
  inline bool operator!=(const unordered_map<Key, Tp, HashFcn, EqualKey> &lhs,
                         const unordered_map<Key, Tp, HashFcn, EqualKey> &rhs)
  {
    return lhs.rep != rhs.rep;
  }

}

#endif // !ZFWSTL_UNORDERED_MAP_H_