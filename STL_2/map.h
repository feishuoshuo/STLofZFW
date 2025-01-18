#ifndef ZFWSTL_MAP_H_
#define ZFWSTL_MAP_H_
/**
 * map: key, value
 * 根据元素的键值key排序
 * map底层红黑树还是使用insert_unique()
 */
#include <cstddef>             //for size_t, ptrdiff_t
#include "../src/functional.h" //for binary_function, less,select1st
#include "rb_tree.h"
#include "../src/util.h" //for  pair<iterator, bool>
namespace zfwstl
{

  template <class Key, class T, class Compare = zfwstl::less<Key> /* ,class Alloc=alloc */>
  class map
  {
  public:
    typedef Key key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef zfwstl::pair<const Key, T> value_type; // value是可以之后修改的，但是key！！不行
    typedef Compare key_compare;
    // TAG: 嵌套类，以下定义一个functor比较函数对象，其作用就是调用 "元素比较函数"
    class value_compare : public binary_function<value_type, value_type, bool>
    {
      friend class map<Key, T, Compare>;

    private:
      Compare comp;
      // TAG: 构造函数私有化：不允许在类的外部直接通过构造函数创建类的实例
      /**
       * 构造函数是私有的，并且只接受一个Compare类型的参数。
       * 这意味着value_compare对象只能由map类内部创建，这是通过friend class声明实现的，
       * 使得map类可以访问value_compare的私有成员和构造函数
       */
      value_compare(Compare c) : comp(c) {}

    public:
      bool operator()(const value_type &lhs, const value_type &rhs) const
      {
        return comp(lhs.first, rhs.first); // 比较键值的大小
      }
    };

  private:
    typedef rb_tree<key_type, value_type, zfwstl::select1st<value_type>, key_compare> rep_type;
    rep_type t; // 采用红黑树表现map
  public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::iterator iterator; // 注意与set区别，因为map的value值不参与红黑树节点排布，因此可改变
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    map() : t(Compare()) {}
    explicit map(const Compare &comp) : t(comp) {}
    // map不允许相同键值存在->insert_unique
    template <class InputIter>
    map(InputIter first, InputIter last) : t(Compare()) { t.insert_unique(first, last); }
    template <class InputIter>
    map(InputIter first, InputIter last, const Compare &comp) : t(comp) { t.insert_unique(first, last); }
    map(std::initializer_list<value_type> ilist) : t()
    {
      t.insert_unique(ilist.begin(), ilist.end());
    }
    // 拷贝构造
    map(const map &rhs) : t(rhs.t) {}
    // 移动构造
    map(map &&rhs) noexcept : t(zfwstl::move(rhs.t)) {}

    map &operator=(const map &x)
    {
      t = x.t;
      return *this;
    }
    map &operator=(const map &&x)
    {
      t = zfwstl::move(x.t);
      return *this;
    }
    map &operator=(std::initializer_list<value_type> ilist)
    {
      t.clear();
      t.insert_unique(ilist.begin(), ilist.end());
      return *this;
    }

  public:
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return value_compare(t.key_comp()); }
    iterator begin() { return t.begin(); }
    const_iterator begin() const { return t.begin(); }
    iterator end() { return t.end(); }
    const_iterator end() const { return t.end(); }
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
    /**
     * value_type(k, T()):创建一个value_type对象
     * (insert(...)).first：获取insert返回的szfwstl::pair中的第一个元素，即指向插入元素的迭代器
     * *((...))：通过迭代器解引用，获取对应的value_type对象
     */
    T &operator[](const key_type &k) { return (*((insert(value_type(k, T()))).first)).second; }
    void swap(map &x) { t.swap(x.t); }
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

    iterator find(const key_type &x) { return t.find(x); }
    const_iterator find(const key_type &x) const { return t.find(x); }
    size_type count(const key_type &x) const { return t.count_unique(x); }
    iterator lower_bound(const key_type &x) { return t.lower_bound(x); }
    iterator upper_bound(const key_type &x) { return t.upper_bound(x); }
    const_iterator lower_bound(const key_type &x) const { return t.lower_bound(x); }
    const_iterator upper_bound(const key_type &x) const { return t.upper_bound(x); }
    zfwstl::pair<iterator, iterator>
    equal_range(const key_type &x) { return t.equal_range(x); }
    zfwstl::pair<const_iterator, const_iterator>
    equal_range(const key_type &x) const { return t.equal_range(x); }

  public:
    friend bool operator==(const map &lhs, const map &rhs) { return lhs.t == rhs.t; }
    friend bool operator<(const map &lhs, const map &rhs) { return lhs.t < rhs.t; }
  };

  // 重载比较操作符
  template <class Key, class T, class Compare>
  bool operator==(const map<Key, T, Compare> &lhs, const map<Key, T, Compare> &rhs)
  {
    return lhs.t == rhs.t;
  }
  template <class Key, class T, class Compare>
  bool operator<(const map<Key, T, Compare> &lhs, const map<Key, T, Compare> &rhs)
  {
    return lhs.t < rhs.t;
  }
  template <class Key, class T, class Compare>
  bool operator!=(const map<Key, T, Compare> &lhs, const map<Key, T, Compare> &rhs)
  {
    return !(lhs == rhs);
  }
  template <class Key, class T, class Compare>
  bool operator>(const map<Key, T, Compare> &lhs, const map<Key, T, Compare> &rhs)
  {
    return rhs < lhs;
  }
  template <class Key, class T, class Compare>
  bool operator<=(const map<Key, T, Compare> &lhs, const map<Key, T, Compare> &rhs)
  {
    return !(rhs < lhs);
  }
  template <class Key, class T, class Compare>
  bool operator>=(const map<Key, T, Compare> &lhs, const map<Key, T, Compare> &rhs)
  {
    return !(lhs < rhs);
  }
}

#endif // !ZFWSTL_MAP_H_