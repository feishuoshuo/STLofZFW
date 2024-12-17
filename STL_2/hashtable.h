#ifndef ZFWSTL_HASHTABLE_H_
#define ZFWSTL_HASHTABLE_H_
/**
 * hashtable哈希表
 * hash冲突采用开链法
 * hash的迭代器没有后退操作(operator--())，即它没有定义所谓逆向迭代器
 */
#include <cstddef>                   //for size_t, ptrdiff_t
#include "../src/iterator.h"         //for forward_iterator_tag
#include "../src/memory/allocator.h" //simple_allocator标准空间支配其
#include "../src/memory/construct.h"
#include "../STL/vector.h"
#include "../src/algorithms/algobase.h" //for lower_bound
namespace zfwstl
{
  /**
   * bucket所维护的linked list并不采用STL的list或者slist，
   * 而是自行维护一个hash table node.
   */
  template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey>
  class hashtable;
  template <class Value>
  struct __hashtable_node
  {
    __hashtable_node *next;
    Value val;
  };
  template <class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey /*, class Alloc */>
  struct __hashtable_iterator
  {
    typedef zfwstl::hashtable<Value, Key, HashFcn, ExtractKey, EqualKey> hashtable;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey> iterator;
    // typedef __hashtable_const_iterator< Value,  Key,  HashFcn,  ExtractKey,  EqualKey> const_iterator;
    typedef __hashtable_node<Value> node;
    typedef forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef Value &reference;
    typedef Value *pointer;

    node *cur;     // 迭代器目前所指节点
    hashtable *ht; // 保持对容器的连结关系(因为可能需要从bucket到另一个bucket)
    __hashtable_iterator(node *n, hashtable *tab) : cur(n), ht(tab) {}
    __hashtable_iterator() {}
    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); }
    // 其前进操作是首先尝试从目前所指结点出发，前进一个位置
    iterator &operator++()
    {
      const node *old = cur;
      cur = cur->next;
      if (!cur)
      {
        // 若目前cur节点为list尾端，就跳至下一个bucket身上，即指向下一个list的头部节点
        size_type bucket = ht->bkt_num(old->val);
        while (!cur && ++bucket < (ht->get_buckets()).size())
          cur = ht->get_buckets()[bucket];
      }
      return *this;
    }
    iterator &operator++(int)
    {
      iterator tmp = *this;
      ++*this;
      return tmp;
    }

    bool operator==(const iterator &it) const { return cur == it.cur; }
    bool operator!=(const iterator &it) const { return cur != it.cur; }
  };

  /**
   * 虽然开链法不要求表格大小必须为质数，
   * 但SGI STL仍然以质数来设计表格大小，且将28个质数(逐渐呈现大约两倍关系)计算好，
   * 以备随时访问，同时提供一个函数，用以查询这28个质数中，“最接近某数并大于某数”的质数
   */
  static const int __stl_num_primes = 28;
  static const unsigned long __stl_prime_list[__stl_num_primes] = {
      53ul, 97ul, 193ul, 389ul, 769ul,
      1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
      49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
      1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
      50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
      1610612741ul, 3221225473ul, 4294967291ul};
  inline unsigned long __stl_next_prime(unsigned long n)
  {
    const unsigned long *first = __stl_prime_list;
    const unsigned long *last = __stl_prime_list + __stl_num_primes;
    const unsigned long *pos = zfwstl::lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
  }
  //=============================hashtable================================
  template <class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey /*, class Alloc */>
  class hashtable
  {
  public:
    // ExtractKey 从节点中去除键值方法(函数/仿函数)
    typedef HashFcn hasher;     // hash函数的函数型别-->计算元素位置，即bucket位置的函数
    typedef EqualKey key_equal; // 判断键值相同与否方法(函数/仿函数)

    typedef Key key_type;
    typedef Value value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef __hashtable_iterator<value_type, key_type, hasher, ExtractKey, key_equal> iterator;
    // typedef __hashtable_iterator<value_type, key_type, hasher,ExtractKey,key_equal> const_iterator;

  private:
    // TODO：stl_hash_fun.h  以下三者均为函数对象，stl_hash_fun.h定义有数个标准型别的hasher
    hasher hash;
    key_equal equals;
    ExtractKey get_key;
    typedef __hashtable_node<Value> node;
    // 专属空间配置器
    typedef zfwstl::simple_allocator<node /*,Alloc */> node_allocator;
    typedef zfwstl::simple_allocator<node /*,Alloc */> allocator_type;

    zfwstl::vector<node * /*,Alloc */> buckets;
    size_type num_elements; // 元素个数

  public:
    hashtable(size_type n, const hasher &hf, const key_equal &eql) : hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0)
    {
      initialize_buckets(n);
    }
    ~hashtable() { clear(); }

    // 获取桶访问
    zfwstl::vector<node *> &get_buckets() { return buckets; }
    // 迭代器相关操作
    iterator begin()
    {
      for (size_type n = 0; n < buckets.size(); ++n)
      {
        if (buckets[n]) // 找到第一个有节点的位置就返回
          return iterator(buckets[n], this);
      }
      return iterator(nullptr, this);
    }
    // const_iterator begin() const
    // {
    //   for (size_type n = 0; n < buckets.size(); ++n)
    //   {
    //     if (buckets[n]) // 找到第一个有节点的位置就返回
    //       return iterator(buckets[n], this);
    //   }
    //   return iterator(nullptr, this);
    // }
    iterator end()
    {
      return iterator(nullptr, this);
    }
    // const_iterator end() const
    // {
    //   return const_iterator(nullptr, const_cast<hashtable *>(this));
    // }

    // const_iterator cbegin() const noexcept
    // {
    //   return begin();
    // }
    // const_iterator cend() const noexcept
    // {
    //   return end();
    // }

    // 容量相关操作
    bool empty() const noexcept { return num_elements == 0; }
    size_type size() const noexcept { return num_elements; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }
    // bucket个数，即buckets vector大小
    size_type bucket_count() const { return buckets.size(); }
    size_type max_bucket_count() const { return __stl_prime_list[__stl_num_primes - 1]; }
    allocator_type get_allocator() { return node_allocator(); }

  public:
    // insert, resize表格重整
    std::pair<iterator, bool> insert_unique(const value_type &obj)
    {
      resize(num_elements + 1); // 判断是否需要重建表格，如需则扩充
      return insert_unique_noresize(obj);
    }
    iterator insert_equal(const value_type &obj)
    {
      resize(num_elements + 1); // 判断是否需要重建表格，如需则扩充
      return insert_equal_noresize(obj);
    }

    // copy_from, clear
    void clear()
    {
      for (size_type i = 0; i < buckets.size(); ++i)
      {
        node *cur = buckets[i];
        while (cur)
        {
          node *next = cur->next;
          delete_node(cur);
          cur = next;
        }
        buckets[i] = nullptr;
      }
      num_elements = 0;
      // PS! buckets vector并没释放掉空间，仍保持原有大小
    }
    void copy_from(const hashtable &ht)
    {
      // 先清除自己的buckets vector
      buckets.clear();
      // 保留自己的buckets vector的空间，并使其与对方ht空间相同
      buckets.reserve(ht.buckets.size());
      buckets.insert(buckets.end(), ht.buckets.size(), static_cast<node *>(0));
      try
      {
        for (size_type i = 0; i < ht.buckets.size(); ++i)
        {
          if (const node *cur = ht.buckets[i])
          {
            node *copy = new_node(cur->val);
            buckets[i] = copy;
            // 针对同一个bucket list，复制每一个节点
            for (node *next = cur->next; next; cur = next, next = cur->next)
            {
              copy->next = new_node(next->val);
              copy = copy->next;
            }
          }
        }
        num_elements = ht.num_elements; // 重新登录节点个数(hashtable的大小)
      }
      catch (...)
      {
        clear();
      }
    }
    size_type elems_in_bucket(size_type bucket) const
    {
      size_type result = 0;
      for (node *n = buckets[bucket]; n; n = n->next)
        ++result;
      return result;
    }

  private:
    // hashtable的构造与内存管理
    node *new_node(const value_type &obj)
    {
      node *n = node_allocator::allocate();
      n->next = nullptr;
      try
      {
        zfwstl::construct(&n->val, obj); // 在 n->val 的地址上构造对象
        return n;
      }
      catch (...)
      {
        node_allocator::deallocate(n);
      }
    }
    void delete_node(node *n)
    {
      zfwstl::destroy(&n->val);
      node_allocator::deallocate(n);
    }
    size_type next_size(size_type n) const { return __stl_next_prime(n); }
    void initialize_buckets(size_type n)
    {
      const size_type n_buckets = next_size(n); // next_size()返回最接近 n且 >n 的质数
      // 例如，n=50，返回53,余下3个填 0
      buckets.reserve(n_buckets);
      buckets.insert(buckets.end(), n_buckets, static_cast<node *>(0));
      num_elements = 0;
    }
    void resize(size_type num_elements_hint)
    {
      /**
       * 判断原则：拿元素个数(加入新增元素后)和bucket vector的大小比较，
       * >大于：重建表格
       * 由此可判，每个bucket(list)的最大容量和bucket vector的大小相同
       */
      const size_type old_n = buckets.size();
      if (num_elements_hint > old_n)
      {
        const size_type n = next_size(num_elements_hint); // 找出下一个质数
        if (n > old_n)
        {
          zfwstl::vector<node *> tmp(n, static_cast<node *>(0)); // 设置新的buckets
                                                                 // 处理每一个旧的bucket
          for (size_type bucket = 0; bucket < old_n; ++bucket)
          {
            node *first = buckets[bucket];
            while (first)
            {
              size_type new_bucket = bkt_num(first->val, n); // 找出节点落在哪个新的bucket内
              // 1-令旧bucket 指向其所对应之串行的下一个节点(以便迭代处理)
              buckets[bucket] = first->next;
              // 2+3-将当前节点插入新bucket 内，成为其对应串行的第一个节点
              first->next = tmp[new_bucket];
              // 4-回到旧bucket 所指的待处理串行，准备处理下一个节点
              first = buckets[bucket];
            }
          }
          buckets.swap(tmp); // vector.swap 新旧两个buckets对调
          // 离开时释放local tmp内存
        }
      }
    }
    std::pair<iterator, bool> insert_unique_noresize(const value_type &obj)
    {
      // 1-横找
      const size_type n = bkt_num(obj); // 决定obj应位于#n bucket
      node *first = buckets[n];
      // 2-纵找
      for (node *cur = first; cur; cur = cur->next)
        if (equals(get_key(cur->val), get_key(obj)))
          return std::pair<iterator, bool>(iterator(cur, this), false); // 若发现与链表中某键值相同，则不插入，立即返回

      node *tmp = new_node(obj);
      tmp->next = first; // 头插法
      buckets[n] = tmp;
      ++num_elements;
      return std::pair<iterator, bool>(iterator(tmp, this), true);
    }
    iterator insert_equal_noresize(const value_type &obj)
    {
      // 1-横找
      const size_type n = bkt_num(obj); // 决定obj应位于#n bucket
      node *first = buckets[n];
      // 2-纵找
      for (node *cur = first; cur; cur = cur->next)
        if (equals(get_key(cur->val), get_key(obj)))
        {
          // 若发现与链表中某键值相同，则马上插入，立即返回
          node *tmp = new_node(obj);
          tmp->next = cur->next; // 将新节点插入目前位置
          cur->next = tmp;
          ++num_elements;
          return iterator(tmp, this);
        }
      // 没有发现重复键
      node *tmp = new_node(obj);
      tmp->next = first; // 头插法
      buckets[n] = tmp;
      ++num_elements;
      return iterator(tmp, this);
    }

  public:
    // 判断元素的落脚地 hash散列函数
    // 1: 接受实值， buckets个数
    size_type bkt_num(const value_type &obj, size_type n) const { return bkt_num_key(get_key(obj), n); }
    // 2: 接受实值
    size_type bkt_num(const value_type &obj) const { return bkt_num_key(get_key(obj)); }
    // 3: 只接受键值
    size_type bkt_num_key(const key_type &key) const { return bkt_num_key(key, buckets.size()); }
    // 4: 接受键值， buckets个数
    size_type bkt_num_key(const key_type &key, size_type n) const
    {
      return hash(key) % n; // SGI的所有内建的hash()
    };
  };
}

#endif // !ZFWSTL_HASHTABLE_H_