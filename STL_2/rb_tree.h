#ifndef ZFWSTL_RB_TREE_H_
#define ZFWSTL_RB_TREE_H_
/**
 * RB tree红黑树
 * 左根右 根叶黑 不红红 黑路同
 */
#include <cstddef>                   //for size_t, ptrdiff_t
#include "../src/iterator.h"         //for bidirectional_iterator_tag, distance
#include "../src/memory/allocator.h" //标准空间配置器
#include "../src/util.h"             //for pair<iterator,bool>, swap
#include "../src/exceptdef.h"        //for THROW_LENGTH_ERROR_IF
namespace zfwstl
{
  typedef bool __rb_tree_color_type;
  const __rb_tree_color_type __rb_tree_red = false;  // 红=0
  const __rb_tree_color_type __rb_tree_black = true; // 黑=1

  //==================节点node==================
  struct __rb_tree_node_base
  {
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node_base *base_ptr;

    color_type color; // 节点颜色
    // 由于红黑树的各种操作时常需要上溯其父节点，所以需要parent指针
    base_ptr parent; // 父节点
    base_ptr left;
    base_ptr right;

    static base_ptr minimum(base_ptr x)
    {
      // 二叉搜索树特性，向左找最小值
      while (x->left)
        x = x->left;
      return x;
    }
    static base_ptr maximum(base_ptr x)
    {
      // 二叉搜索树特性，向右找最大值
      while (x->right)
        x = x->right;
      return x;
    }
  };
  template <class Value>
  struct __rb_tree_node : public __rb_tree_node_base
  {
    typedef __rb_tree_node<Value> *link_type;
    Value value_field; // 节点值
  };
  //==================迭代器iterator==================
  struct __rb_tree_base_iterator
  {
    typedef __rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;

    base_ptr node; // 用来与容器之间产生一个连接关系 make a refernce

    // RB tree迭代器的前进操作operator++()就是调用它
    void increment()
    {
      if (node->right)
      {
        node = node->right;
        while (node->left)
          node = node->left;
      }
      else
      {
        base_ptr y = node->parent;
        while (node == y->right)
        {
          node = y;
          y = y->parent;
        }
        if (node->right != y)
          node = y;
      }
    }
    // RB tree迭代器的后退操作operator--()就是调用它
    void decrement()
    {
      // NOTE:当 node 是头节点时，递减操作让 node 指向右节点是为了找到树中的最大值节点，即当前节点的前驱节点
      if (node->color == __rb_tree_red && node->parent->parent == node)
        node = node->right;
      else if (node->left)
      {
        base_ptr y = node->left;
        while (y->right)
          y = y->right;
        node = y;
      }
      else
      {
        base_ptr y = node->parent;
        while (node == y->left)
        {
          node = y;
          y = y->parent;
        }
        node = y;
      }
    }

    bool operator==(const __rb_tree_base_iterator &rhs) { return node == rhs.node; }
    bool operator!=(const __rb_tree_base_iterator &rhs) { return node != rhs.node; }
  };
  template <class Value, class Ref, class Ptr>
  struct __rb_tree_iterator : public __rb_tree_base_iterator
  {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __rb_tree_iterator<Value, Value &, Value *> iterator;
    typedef __rb_tree_iterator<Value, const Value &, const Value *> const_iterator;
    typedef __rb_tree_iterator<Value, Ref, Ptr> self;
    typedef __rb_tree_node<Value> *link_type;

    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type x) { node = x; }
    __rb_tree_iterator(const iterator &rhs) { node = rhs.node; }
    __rb_tree_iterator(const const_iterator &rhs) { node = rhs.node; }

    // TAG：强制类型转换 static_cast, dynamic_cast
    /**
     * static_cast：
     * 用于执行非多态类型的转换，也就是说，它不涉及运行时类型检查。
     * 它主要用于基本数据类型之间的转换（如 int 到 double），
     * 以及类层次结构中的向上转换（从派生类到基类）和某些向下转换（从基类到派生类，但前提是编译器知道这种转换是安全的）。
     *
     * dynamic_cast：
     * 用于类层次结构中的向下转换（从基类到派生类），并且涉及运行时类型检查。
     * 如果转换失败，dynamic_cast 会返回 nullptr（对于指针类型）或抛出异常（对于引用类型），
     * 因此它比 static_cast 更安全。
     * 例子：Base* b = new Base(); Derived* d = dynamic_cast<Derived*>(b);
     */
    reference operator*() const { return static_cast<link_type>(node)->value_field; }
    pointer operator->() const { return &(operator*()); }
    self &operator++()
    {
      increment();
      return *this;
    }
    self operator++(int)
    {
      self tmp = *this;
      increment();
      return tmp;
    }
    self &operator--()
    {
      decrement();
      return *this;
    }
    self operator--(int)
    {
      self tmp = *this;
      decrement();
      return tmp;
    }
  };
  //===============================rb_tree==========================
  // KeyOfValue用于从值类型中提取Key
  template <class Key, class Value, class KeyOfValue, class Compare /* ,class Alloc=alloc */>
  class rb_tree
  {
  protected:
    typedef void *void_pointer;
    typedef __rb_tree_node_base *base_ptr;
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node<Value> rb_tree_node;
    typedef zfwstl::simple_allocator<rb_tree_node /* , Alloc */> rb_tree_node_allocator; // 专属之空间配置器，每次分配一个节点大小
  public:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef rb_tree_node *link_type;

    typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
    typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

  protected:
    size_type node_count; // 记录数的大小=节点个数
    link_type header;
    Compare key_compare; // 节点间的键值大小比较准则，是个function object

    // 下面三个函数用来方便取得header的成员
    // TAG:static_cast<link_type &>不适用于这里
    link_type &root() const { return (link_type &)(header->parent); } // 返回的是对指针的引用 link_type &
    link_type &leftmost() const { return (link_type &)(header->left); }
    link_type &rightmost() const { return (link_type &)(header->right); }
    // 用来方便取得节点x的成员
    // TAG:静态成员函数属于类本身，而不是类的某个特定对象。这意味着你不需要创建类的实例就可以调用静态成员函数
    static link_type &left(link_type x) { return (link_type &)(x->left); }
    static link_type &right(link_type x) { return (link_type &)(x->right); }
    static link_type &parent(link_type x) { return (link_type &)(x->parent); }
    static color_type &color(link_type x) { return x->color; }
    static reference value(link_type x) { return x->value_field; }
    // TAG: KeyOfValue()这是一个模板函数，它接受一个 value_type 引用类型的对象，并从中提取出一个 Key 类型的键。这个函数通常是一个函数对象或者一个模板函数，它定义了如何从值中提取键
    static const Key &key(link_type x) { return KeyOfValue()(value(x)); }
    // 也是用来方便取得节点x的成员[函数重载]
    static link_type &left(base_ptr x) { return (link_type &)(x->left); }
    static link_type &right(base_ptr x) { return (link_type &)(x->right); }
    static link_type &parent(base_ptr x) { return (link_type &)(x->parent); }
    static color_type &color(base_ptr x) { return x->color; }
    static reference value(base_ptr x) { return static_cast<link_type>(x)->value_field; }
    static const Key &key(base_ptr x) { return KeyOfValue()(value(static_cast<link_type>(x))); }

    static link_type minimum(link_type x) { return static_cast<link_type>(__rb_tree_node_base::minimum(x)); }
    static link_type maximum(link_type x) { return static_cast<link_type>(__rb_tree_node_base::maximum(x)); }

    link_type get_node() { return rb_tree_node_allocator::allocate(1); }
    void pop_node(link_type p) { rb_tree_node_allocator::deallocate(p); }
    link_type create_node(const value_type &x)
    {
      link_type tmp = get_node(); // 配置空间
      try
      {
        zfwstl::construct(&tmp->value_field, x); // 构造内容
        tmp->left = nullptr;
        tmp->right = nullptr;
        tmp->parent = nullptr;
      }
      catch (...)
      {
        pop_node(tmp);
        throw;
      }
      return tmp;
    }
    link_type clone_node(link_type x)
    {
      // 复制一个节点(值和色)
      link_type tmp = create_node(x->value_field);
      tmp->color = x->color;
      tmp->right = nullptr;
      tmp->left = nullptr;
      return tmp;
    }
    void destroy_node(link_type p)
    {
      zfwstl::destroy(&p->value_field); // 析构内容
      pop_node(p);                      // 释放内存
    }

  public:
    // 构造、复制、移动、析构函数
    // default constructor
    rb_tree(const Compare &comp = Compare()) : node_count(0), key_compare(comp) { init(); }
    ~rb_tree()
    {
      clear();
      pop_node(header);
    }
    //=================operator操作运算符重载=====================
    // TODO：操作符重载=
    rb_tree &operator=(const rb_tree &rhs);

    void swap(rb_tree &rhs)
    {
      if (this != &rhs)
      {
        zfwstl::swap(header, rhs.header);
        zfwstl::swap(node_count, rhs.node_count);
        zfwstl::swap(key_compare, rhs.key_compare);
      }
    }

    // 容量相关操作
    Compare
    key_comp() const
    {
      return key_compare;
    }
    bool empty() const noexcept { return node_count == 0; }
    size_type size() const noexcept { return node_count; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); } // 理论最大大小
    iterator begin() { return iterator(leftmost()); /*使用iterator构造函数*/ } // RB树的起头为最小/左节点处
    iterator end() { return header; }
    const_iterator begin() const { return const_iterator(leftmost()); }
    const_iterator end() const { return const_iterator(header); }
    iterator rend();
    iterator rbegin();

    iterator find(const Key &k)
    {
      auto y = header;
      auto x = root();
      while (x)
      {
        if (!key_compare(key(x), k))
        {
          // key 小于等于 x 键值，向左走
          /**
           * 这里的逗号,用于在if语句的代码块中分隔两个语句。
           * 在C++中，逗号运算符会顺序执行两个表达式，并返回最后一个表达式的结果。
           * 在这个上下文中，逗号运算符被用来同时更新y和x的值。
           */
          y = x, x = x->left;
        }
        else // key 大于 x 键值，向右走
          x = right(x);
      }
      iterator j = iterator(y);
      return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }
    const_iterator find(const Key &k) const
    {
      link_type y = header;
      link_type x = root();
      while (x)
      {
        if (!key_compare(key(x), k))
        {
          // key 小于等于 x 键值，向左走
          /**
           * 这里的逗号,用于在if语句的代码块中分隔两个语句。
           * 在C++中，逗号运算符会顺序执行两个表达式，并返回最后一个表达式的结果。
           * 在这个上下文中，逗号运算符被用来同时更新y和x的值。
           */
          y = x, x = x->left;
        }
        else // key 大于 x 键值，向右走
          x = right(x);
      }
      const_iterator j = const_iterator(y);
      return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }
    void clear()
    {
      if (node_count != 0)
      {
        erase_since(root());
        leftmost() = header;
        root() = nullptr;
        rightmost() = header;
        node_count = 0;
      }
    }
    void erase(iterator hint)
    {
      auto y = rb_tree_erase_rebalance(hint.node, header->parent, header->left, header->right);
      destroy_node(static_cast<link_type>(y));
      --node_count;
    }
    void erase(iterator __first, iterator __last)
    {
      if (__first == begin() && __last == end())
        clear();
      else
        while (__first != __last)
          erase(__first++);
    }
    // 删除键值等于 key 的元素，返回删除的个数
    size_type erase(const key_type &x)
    {
      auto __p = equal_range(x);
      auto n = zfwstl::distance(__p.first, __p.second);
      erase(__p.first, __p.second);
      return n;
    }

    iterator copy(iterator x, iterator p);
    // 统计某个给定键（key）在容器中出现的唯一次数
    size_type count_unique(const key_type &key) const
    {
      return find(key) != end() ? 1 : 0;
    }
    size_type count_multi(const key_type &key) const
    {
      auto p = equal_range_multi(key);
      return static_cast<size_type>(zfwstl::distance(p.first, p.second));
    }
    //=====================修改红黑树相关操作insert, erase=====================
    // 被插入节点的key在整棵树中，必须是独一无二的
    // 若树中已有相同key，插入操作不会真正进行
    // TODO:有问题！！！pair
    std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator, bool>
    insert_unique(const value_type &v)
    {
      link_type y = header;
      link_type x = root();
      bool comp = true;
      while (x)
      {
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x));
        x = comp ? left(x) : right(x);
      }
      // 找到y是插入点的父节点，此时y必然是叶子节点
      iterator j = iterator(y); // 令迭代器j指向y节点
      if (comp)
      {
        if (j == begin())
          return std::pair<iterator, bool>(__insert(x, y, v), true);
        else
          --j;
      }
      if (key_compare(key(j.node), KeyOfValue()(v))) // j指向的节点值小于v值，x新插入点在右子节点
        return std::pair<iterator, bool>(__insert(x, y, v), true);

      // 至此，新值v一定与树中键值重复，则不插入
      return std::pair<iterator, bool>(j, false);
    }
    void insert_unique(const_iterator __first, const_iterator __last)
    {
      for (; __first != __last; ++__first)
        insert_unique(*__first);
    }
    void insert_unique(iterator __first, iterator __last)
    {
      for (; __first != __last; ++__first)
        insert_unique(*__first);
    }
    template <class InputIterator>
    void insert_unique(InputIterator first, InputIterator last)
    {
      size_type n = zfwstl::distance(first, last);
      THROW_LENGTH_ERROR_IF(node_count > max_size() - n, "rb_tree<T, Comp>'s size too big");
      for (; n > 0; --n, ++first)
        insert_unique(*first);
    }
    // 可插入重复key
    iterator insert_equal(const value_type &v)
    {
      link_type y = header; // 根节点的父节点，即空白节点header
      link_type x = root(); // 从根节点开始
      while (x)
      {
        y = x;
        // 遇“大”则往左，遇“小/等于”则往右
        // TAG: key_compare(KeyOfValue()(v), key(x)) 是一个比较操作，它比较新值 v 的键和当前节点 x 的键
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
      }
      return __insert(x, y, v); // x新插入点，y插入点父节点，v新值
    }
    void insert_equal(const_iterator __first, const_iterator __last)
    {
      for (; __first != __last; ++__first)
        insert_equal(*__first);
    }
    void insert_equal(iterator __first, iterator __last)
    {
      for (; __first != __last; ++__first)
        insert_equal(*__first);
    }
    template <class InputIterator>
    void insert_equal(InputIterator first, InputIterator last)
    {
      size_type n = zfwstl::distance(first, last);
      THROW_LENGTH_ERROR_IF(node_count > max_size() - n, "rb_tree<T, Comp>'s size too big");
      for (; n > 0; --n, ++first)
        insert_equal(*first);
    }
    //==========================
    size_type count(const key_type &k) const
    {
      // TODO
      auto p = equal_range(k);
      return static_cast<size_type>(zfwstl::distance(p.first, p.second));
      ;
    }
    iterator lower_bound(const key_type &k)
    {
      link_type y = header; // 最后一个不小于K的节点
      link_type x = root(); // 当前节点

      while (x != 0)
        if (!key_compare(key(x), k))
          y = x, x = left(x);
        else
          x = right(x);

      return iterator(y);
    }
    const_iterator lower_bound(const key_type &k) const
    {
      link_type y = header; // 最后一个不小于K的节点
      link_type x = root(); // 当前节点

      while (x != 0)
        if (!key_compare(key(x), k))
          y = x, x = left(x);
        else
          x = right(x);

      return iterator(y);
    }
    iterator upper_bound(const key_type &k)
    {
      link_type y = header;
      link_type x = root();

      while (x != 0)
        if (key_compare(k, key(x)))
          y = x, x = left(x);
        else
          x = right(x);

      return iterator(y);
    }
    const_iterator upper_bound(const key_type &k) const
    {
      link_type y = header;
      link_type x = root();

      while (x != 0)
        if (key_compare(k, key(x)))
          y = x, x = left(x);
        else
          x = right(x);

      return iterator(y);
    }
    std::pair<iterator, iterator> equal_range(const key_type &k)
    {
      return std::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }
    std::pair<const_iterator, const_iterator> equal_range(const key_type &k) const
    {
      return std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
    }

  private:
    iterator __insert(base_ptr x_, base_ptr y_, const value_type &v)
    {
      link_type x = static_cast<link_type>(x_);
      link_type y = static_cast<link_type>(y_);
      link_type z;
      if (y == header || x || key_compare(KeyOfValue()(v), key(y)))
      {
        z = create_node(v);
        left(y) = z;
        if (y == header)
        {
          root() = z;
          rightmost() = z;
        }
        else if (y == leftmost())
          leftmost() = z;
      }
      else
      {
        z = create_node(v);
        right(y) = z;
        if (y == rightmost())
          rightmost() = z;
      }
      parent(z) = y;
      left(z) = nullptr;
      right(z) = nullptr;
      __rb_tree_rebalance(z, header->parent); //(新增节点, root)
      ++node_count;
      return iterator(z); // 构造一个迭代器而不是强制类型转换
    }
    void init()
    {
      header = get_node();           // 产生一个节点空间，令header指向它
      color(header) = __rb_tree_red; // 令其为红色
      root() = nullptr;
      leftmost() = header;  // 令header左子节点为自己
      rightmost() = header; // 令header右子节点为自己
    }
    // 从 x 节点开始删除该节点及其子树
    void erase_since(base_ptr x)
    {
      while (x)
      {
        erase_since(x->right);
        auto y = x->left;
        destroy_node(static_cast<link_type>(x));
        x = y;
      }
    }

    inline void __rb_tre_rotate_left(__rb_tree_node_base *x, __rb_tree_node_base *&root)
    {
      // x为旋转点
      __rb_tree_node_base *y = x->right; // y为中心点
      x->right = y->left;
      if (y->left)
        y->left->parent = x; // 别忘了回马枪设定父节点
      y->parent = x->parent;
      // 令 y完全顶替x地位
      if (x == root) // x为根节点
        root = y;
      else if (x == x->parent->left) // x为其父节点的左子节点
        x->parent->left = y;
      else
        x->parent->right = y;
      y->left = x;
      x->parent = y;
    }
    inline void __rb_tre_rotate_right(__rb_tree_node_base *x, __rb_tree_node_base *&root)
    {
      // x为旋转点
      __rb_tree_node_base *y = x->left; // y为中心点
      x->left = y->right;
      if (y->right)
        y->right->parent = x; // 别忘了回马枪设定父节点
      y->parent = x->parent;
      // 令 y完全顶替x地位
      if (x == root) // x为根节点
        root = y;
      else if (x == x->parent->right) // x为其父节点的左子节点
        x->parent->right = y;
      else
        x->parent->left = y;
      y->right = x;
      x->parent = y;
    }
    // 调整红黑树，旋转及改变颜色
    void __rb_tree_rebalance(__rb_tree_node_base *x, __rb_tree_node_base *&root)
    {
      x->color = __rb_tree_red; // 新节点必为红色
      while (x != root && x->parent->color == __rb_tree_red)
      {
        if (x->parent == x->parent->parent->left)
        {
          // 父节点为祖父的左子节点
          __rb_tree_node_base *y = x->parent->parent->right; // 当前节点x的叔叔节点
          if (y && y->color == __rb_tree_red)
          {
            // 1.插入节点的叔叔是红色-->叔父爷变色，爷爷变插入节点
            x->parent->color = __rb_tree_black;
            y->color = __rb_tree_black;
            x->parent->parent->color = __rb_tree_red;
            x = x->parent->parent;
          }
          else
          {
            // 2.插入节点的叔叔是黑色-->(LL LR)旋转，然后变色
            if (x == x->parent->right)
            {
              x = x->parent;
              __rb_tre_rotate_left(x, root); // x为旋转点
            }
            x->parent->color = __rb_tree_black;
            x->parent->parent->color = __rb_tree_red;
            __rb_tre_rotate_right(x->parent->parent, root); // x->parent->parent为旋转点
          }
        }
        else
        {
          // 父节点为祖父的右子节点
          __rb_tree_node_base *y = x->parent->parent->left; // 当前节点x的叔叔节点
          if (y && y->color == __rb_tree_red)
          {
            // 1.插入节点的叔叔是红色-->叔父爷变色，爷爷变插入节点
            x->parent->color = __rb_tree_black;
            y->color = __rb_tree_black;
            x->parent->parent->color = __rb_tree_red;
            x = x->parent->parent;
          }
          else
          {
            // 2.插入节点的叔叔是黑色-->(RR RL)旋转，然后变色
            if (x == x->parent->left)
            {
              x = x->parent;
              __rb_tre_rotate_right(x, root); // x为旋转点
            }
            x->parent->color = __rb_tree_black;
            x->parent->parent->color = __rb_tree_red;
            __rb_tre_rotate_left(x->parent->parent, root); // x->parent->parent为旋转点
          }
        }
      }
      // while结束
      root->color = __rb_tree_black; // 根节点永远为黑
    }
    base_ptr rb_tree_erase_rebalance(base_ptr z, base_ptr &root, base_ptr &left_m, base_ptr &right_m)
    {
      base_ptr d = z;
      base_ptr r_parent = nullptr; // d节点的孩子节点的父节点
      base_ptr r = nullptr;        // d节点的孩子节点
      // 1-确定替代节点r
      if (!d->left)
        r = d->right;
      else if (!d->right)
        r = d->left;
      else
      { // 右子树的最小节点作为替代节点, 也就是接下来的d的子节点
        d = d->right;
        while (d->left)
          d = d->left;
        r = d->right;
      }
      // 2-重新链接替代节点
      // 暗指删除节点d均有左右孩子：让后继节点替代它
      if (d != z)
      {
        z->left->parent = d;
        d->left = z->left;

        // 如果 d 不是 z 的右子节点，那么 z 的右子节点一定有左孩子
        if (d != z->right)
        { // r 替换 d 的位置
          r_parent = d->parent;
          if (r != nullptr)
            r->parent = d->parent;

          d->parent->left = r;
          d->right = z->right;
          z->right->parent = d;
        }
        else
        {
          r_parent = d;
        }

        // 连接 d 与 z 的父节点
        if (root == z)
          root = d;
        else if (z->parent->left == z)
          z->parent->left = d;
        else
          z->parent->right = d;
        d->parent = z->parent;
        auto tmp = d->color;
        d->color = z->color;
        z->color = tmp;
        d = z;
      }
      else
      {
        r_parent = d->parent;
        if (r)
          r->parent = d->parent;
        if (root == z)
          root = r;
        else if (z->parent->left == z)
          z->parent->left = r;
        else
          z->parent->right = r;
        if (left_m == z)
          if (z->right == 0) // z->left must be null also
            left_m = z->parent;
          // makes left_m == header if z == root
          else
            left_m = minimum(static_cast<link_type>(r));
        if (right_m == z)
          if (z->left == 0) // z->right must be null also
            right_m = z->parent;
          // makes __rightmost == header if z == root
          else // r == z->left
            right_m = maximum(static_cast<link_type>(r));
      }

      if (d->color != __rb_tree_red)
      {
        while (r != root && (!r || r->color == __rb_tree_black))
          if (r == r_parent->left)
          {
            base_ptr __w = r_parent->right;
            if (__w->color == __rb_tree_red)
            {
              __w->color = __rb_tree_black;
              r_parent->color = __rb_tree_red;
              __rb_tre_rotate_left(r_parent, root);
              __w = r_parent->right;
            }
            if ((!__w->left ||
                 __w->left->color == __rb_tree_black) &&
                (!__w->right ||
                 __w->right->color == __rb_tree_black))
            {
              __w->color = __rb_tree_red;
              r = r_parent;
              r_parent = r_parent->parent;
            }
            else
            {
              if (!__w->right ||
                  __w->right->color == __rb_tree_black)
              {
                if (__w->left)
                  __w->left->color = __rb_tree_black;
                __w->color = __rb_tree_red;
                __rb_tre_rotate_right(__w, root);
                __w = r_parent->right;
              }
              __w->color = r_parent->color;
              r_parent->color = __rb_tree_black;
              if (__w->right)
                __w->right->color = __rb_tree_black;
              __rb_tre_rotate_left(r_parent, root);
              break;
            }
          }
          else
          { // same as above, with right <-> left.
            base_ptr __w = r_parent->left;
            if (__w->color == __rb_tree_red)
            {
              __w->color = __rb_tree_black;
              r_parent->color = __rb_tree_red;
              __rb_tre_rotate_right(r_parent, root);
              __w = r_parent->left;
            }
            if ((!__w->right ||
                 __w->right->color == __rb_tree_black) &&
                (!__w->left ||
                 __w->left->color == __rb_tree_black))
            {
              __w->color = __rb_tree_red;
              r = r_parent;
              r_parent = r_parent->parent;
            }
            else
            {
              if (!__w->left ||
                  __w->left->color == __rb_tree_black)
              {
                if (__w->right)
                  __w->right->color = __rb_tree_black;
                __w->color = __rb_tree_red;
                __rb_tre_rotate_left(__w, root);
                __w = r_parent->left;
              }
              __w->color = r_parent->color;
              r_parent->color = __rb_tree_black;
              if (__w->left)
                __w->left->color = __rb_tree_black;
              __rb_tre_rotate_right(r_parent, root);
              break;
            }
          }
        if (r)
          r->color = __rb_tree_black;
      }
      return d;
    }
  };
}

#endif // !ZFWSTL_RB_TREE_H_