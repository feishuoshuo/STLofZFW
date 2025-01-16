#ifndef ZFWSTL_DEQUE_H_
#define ZFWSTL_DEQUE_H_
/**
 * 双向开口的[分段]连续性空间
 * 区别：vector:单向开口的；deque:双向开口的
 * 迭代器：Random Access iterators
 */
#include <cstddef>                       //for size_t, ptrdiff_t
#include "../src/memory/allocator.h"     //标准空间配置器
#include "../src/algorithms/algorithm.h" //for max(), copy(), copy_backward(), fill()
#include "../src/memory/unintialized.h"  //for uninitialized_fill(), uninitialized_copy()
#include "../src/exceptdef.h"            //for MYSTL_DEBUG, THROW_OUT_OF_RANGE_IF
#include "../src/iterator.h"             //for reverse_iterator, distance, is_input_iterator, advance, input_iterator_tag
#include "../src/util.h"                 //for forward
namespace zfwstl
{
  // deque map 初始化的大小
#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif

  template <class T, size_t BufSize = 0>
  struct deque_buf_size
  {
    // static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
    static constexpr size_t value = (BufSize != 0) ? BufSize : (sizeof(T) < 256 ? 4096 / sizeof(T) : 16);
  };

  template <class T, class Ref, class Ptr, size_t BufSize>
  struct __deque_iterator : public iterator<random_access_iterator_tag, T>
  {
    typedef __deque_iterator<T, T &, T *, BufSize> iterator;
    typedef __deque_iterator<T, const T &, const T *, BufSize> const_iterator;
    typedef __deque_iterator /* <T, Ref, Ptr> */ self; // 当前迭代器的类型:自引用迭代器

    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *value_pointer;
    typedef T **map_pointer;

    static const size_type buffer_size = deque_buf_size<T, BufSize>::value; // 缓冲区大小

    // 迭代器所含成员数据 [first, cur)
    value_pointer cur;   // NOTE:cur指向所在缓冲区[1-开头]首元素 ；[2-末尾]尾元素的下一个位置
    value_pointer first; // 指向所在缓冲区的头部
    /**
     * Tip:最后一个缓冲区尚有备用空间。稍后如果有新元素要插入尾端，可直接拿此来用
     */
    value_pointer last; // 指向所在缓冲区的尾部(含备用空间),不是指向最后一个元素！
    map_pointer node;   // 缓冲区所在节点(指向管控中心)

    // 构造、复制、移动函数
    __deque_iterator() noexcept
        : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}

    __deque_iterator(value_pointer v, map_pointer n)
        : cur(v), first(*n), last(*n + buffer_size), node(n) {}

    __deque_iterator(const iterator &rhs)
        : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
    {
    }
    __deque_iterator(iterator &&rhs) noexcept
        : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
    {
      rhs.cur = nullptr;
      rhs.first = nullptr;
      rhs.last = nullptr;
      rhs.node = nullptr;
    }

    __deque_iterator(const const_iterator &rhs)
        : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
    {
    }

    /**
     * 一旦行进时遇到缓冲区边缘，要特别当心！视前进或后退而定，可能需调用以下函数：
     * 跳出一个缓冲区
     */
    void set_node(map_pointer new_node)
    {
      node = new_node;
      first = *new_node;
      last = first + buffer_size;
    }
    // 重载运算符
    reference operator*() const { return *cur; }
    pointer operator->() const { return cur; }
    // TAG：！！注意deque缓冲区直接元素的位置跳跃
    difference_type operator-(const self &x) const
    {
      /**
       * node - x.node：计算两个迭代器所在的缓冲区节点之间的差值；由于每个节点代表一个完整的缓冲区，所以要 * buffer_size
       * static_cast<difference_type>(buffer_size) * (node - x.node)：计算两个迭代器之间完整缓冲区的元素数量。
       * (cur - first) - (x.cur - x.first)：计算两个迭代器在各自缓冲区中的相对位置差。
       */
      return static_cast<difference_type>(buffer_size) * (node - x.node) + (cur - first) - (x.cur - x.first);
    }
    self &operator++()
    {
      ++cur;
      if (cur == last)
      { // 如果到达缓冲区的尾
        set_node(node + 1);
        cur = first;
      }
      return *this;
    }
    self operator++(int)
    {
      self tmp = *this;
      ++*this;
      return tmp;
    }
    self &operator--()
    {
      if (cur == first)
      { // 如果到达缓冲区的头
        set_node(node - 1);
        cur = last;
      }
      --cur;
      return *this;
    }
    self operator--(int)
    {
      self tmp = *this;
      --*this;
      return tmp;
    }
    // operator+=这个操作会直接改变调用它的迭代器对象的状态。
    self &operator+=(difference_type n)
    {
      const auto offset = n + (cur - first);
      if (offset >= 0 && offset < static_cast<difference_type>(buffer_size))
      { // 仍在当前缓冲区
        cur += n;
      }
      else
      { // 要跳到其他的缓冲区
        const auto node_offset = offset > 0
                                     ? offset / static_cast<difference_type>(buffer_size)
                                     : -static_cast<difference_type>((-offset - 1) / buffer_size) - 1;
        set_node(node + node_offset); // 切换到正确map节点，即缓冲区上
        // 在新缓冲区上，移动到正确元素位置
        cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
      }
      return *this;
    }
    // operator+ 这个操作不会改变原始迭代器对象的状态，而是创建并返回一个新的迭代器对象
    self operator+(difference_type n) const
    {
      self tmp = *this;
      return tmp += n; // 调用 operator+=
    }
    self &operator-=(difference_type n)
    {
      return *this += -n; // 调用 operator+=
    }
    self operator-(difference_type n) const
    {
      self tmp = *this;
      return tmp -= n; // 调用 operator-=
    }
    // TAG:*this 是一个指针，它指向当前对象
    reference operator[](difference_type n) const { return *(*this + n); }
    self &operator=(const iterator &rhs)
    {
      if (this != &rhs)
      {
        cur = rhs.cur;
        first = rhs.first;
        last = rhs.last;
        node = rhs.node;
      }
      return *this;
    }
    // 重载比较操作符
    bool operator==(const self &rhs) const { return cur == rhs.cur; }
    bool operator<(const self &rhs) const
    {
      return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node);
    }
    bool operator!=(const self &rhs) const { return !(*this == rhs); }
    bool operator>(const self &rhs) const { return rhs < *this; }
    bool operator<=(const self &rhs) const { return !(rhs < *this); }
    bool operator>=(const self &rhs) const { return !(*this < rhs); }
  };

  //======================================deque===========================================
  template <class T /*,class Alloc=alloc*/, size_t BufSize = 0>
  class deque
  {
  public:
    //  专属空间配置器
    typedef zfwstl::simple_allocator<T> data_allocator; // 每次配置一个元素大小
    typedef zfwstl::simple_allocator<T> allocator_type;
    typedef zfwstl::simple_allocator<T *> map_allocator; // 每次配置一个指针大小
    // list 的嵌套型别定义
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;

    typedef pointer *map_pointer; // 类型：元素的指针的指针(pointer of pointer of T)

    typedef __deque_iterator<T, T &, T *, BufSize> iterator;
    typedef __deque_iterator<T, const T &, const T *, BufSize> const_iterator;
    typedef zfwstl::reverse_iterator<iterator> reverse_iterator;
    typedef zfwstl::reverse_iterator<const_iterator> const_reverse_iterator;

  protected:
    iterator start;  // 指向第一个节点
    iterator finish; // 指向最后一个结点
    /**
     * map是块连续空间，其内的每个元素都是一个指针(即节点)，指向一块缓冲区
     */
    map_pointer map;    // 指向map中控器的指针
    size_type map_size; // map内可容纳多少指针

  public:
    // 构造、复制、移动、析构函数
    // 默认构造函数
    deque() { fill_initialize(0, value_type()); }
    // TAG：以防止类的构造函数参与隐式类型转换。当一个构造函数被声明为 explicit 时，这意味着它不能用于隐式转换，只能用于直接的函数调用和显式构造
    // 参数化构造函数
    explicit deque(size_type n) { fill_initialize(n, value_type()); }
    deque(size_type n, const value_type &value)
    {
      fill_initialize(n, value);
    }
    template <class IIter, typename std::enable_if<zfwstl::is_input_iterator<IIter>::value, int>::type = 0>
    deque(IIter first, IIter last) { copy_init(first, last, iterator_category(first)); }
    // 初始化列表构造函数
    deque(std::initializer_list<value_type> ilist)
    {
      copy_init(ilist.begin(), ilist.end(), zfwstl::forward_iterator_tag());
    }
    // 拷贝构造
    deque(const deque &rhs)
    {
      copy_init(rhs.begin(), rhs.end(), zfwstl::forward_iterator_tag());
    }
    // 移动构造
    deque(deque &&rhs) noexcept
        : start(zfwstl::move(rhs.start)),
          finish(zfwstl::move(rhs.finish)),
          map(rhs.map),
          map_size(rhs.map_size)
    {
      rhs.map = nullptr;
      rhs.map_size = 0;
    }
    ~deque()
    {
      if (map != nullptr)
      {
        clear();
        data_allocator::deallocate(*start.node, start.buffer_size); // 表示start.node指针所指向的内存块中的元素数组的首地址
        *start.node = nullptr;
        map_allocator::deallocate(map, map_size);
        map = nullptr;
      }
    }

    iterator begin() noexcept
    {
      return start;
    }
    const_iterator begin() const noexcept
    {
      return const_iterator(start);
    }
    iterator end() noexcept
    {
      return finish;
    }
    const_iterator end() const noexcept
    {
      return const_iterator(finish);
    }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_iterator cbegin() const noexcept
    {
      return begin();
    }
    const_iterator cend() const noexcept
    {
      return end();
    }
    const_reverse_iterator crbegin() const noexcept
    {
      return rbegin();
    }
    const_reverse_iterator crend() const noexcept
    {
      return const_reverse_iterator(begin());
    }
    // 调用__deque_iterator<>::operator*
    reference front()
    {
      MYSTL_DEBUG(!empty());
      return *start;
    }
    const_reference front() const
    {
      MYSTL_DEBUG(!empty());
      return *begin();
    }
    reference back()
    {
      MYSTL_DEBUG(!empty());
      iterator tmp = finish;
      --tmp;
      return *tmp;
      // 以上为何不改为 return *(finish-1);
      // 因为__deque_iterator<>没有为(finish-1)定义运算子！
    }
    const_reference back() const
    {
      MYSTL_DEBUG(!empty());
      iterator tmp = finish;
      --tmp;
      return *tmp;
    }
    // 容量相关操作
    size_type size() const { return finish - start; }
    size_type max_size() const { return size_type(-1); }
    // 重置容器大小
    void resize(size_type new_size) { resize(new_size, value_type()); }
    void resize(size_type new_size, const value_type &value)
    {
      const auto len = size();
      if (new_size < len)
      {
        erase(start + new_size, finish);
      }
      else
      {
        insert(finish, new_size - len, value);
      }
    }
    bool empty() const { return finish == start; }
    //=================operator操作运算符重载=====================
    // 调用__deque_iterator<>::operator[]
    reference operator[](size_type n)
    {
      MYSTL_DEBUG(n < size());
      return start[static_cast<difference_type>(n)];
    }
    const_reference operator[](size_type n) const
    {
      MYSTL_DEBUG(n < size());
      return start[static_cast<difference_type>(n)];
    }
    // 复制赋值运算符
    deque &operator=(const deque &rhs)
    {
      if (this != &rhs)
      {
        const auto len = size();
        if (len >= rhs.size())
        {
          erase(zfwstl::copy(rhs.start, rhs.finish, start), finish);
        }
        else
        {
          iterator mid = rhs.begin() + static_cast<difference_type>(len);
          zfwstl::copy(rhs.start, mid, start);
          insert(finish, mid, rhs.finish);
        }
      }
      return *this;
    }
    // 移动赋值运算符
    deque &operator=(deque &&rhs)
    {
      clear();
      start = zfwstl::move(rhs.start);
      finish = zfwstl::move(rhs.finish);
      map = rhs.map;
      map_size = rhs.map_size;
      rhs.map = nullptr;
      rhs.map_size = 0;
      return *this;
    }
    deque &operator=(std::initializer_list<value_type> ilist)
    {
      deque tmp(ilist);
      swap(tmp);
      return *this;
    }

    void swap(deque &rhs) noexcept
    {
      if (this != &rhs)
      {
        zfwstl::swap(start, rhs.start);
        zfwstl::swap(finish, rhs.finish);
        zfwstl::swap(map, rhs.map);
        zfwstl::swap(map_size, rhs.map_size);
      }
    }

    // 访问元素相关操作
    reference at(size_type n)
    {
      THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
      return (*this)[n];
    }
    const_reference at(size_type n) const
    {
      THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
      return (*this)[n];
    }

    // 修改容器相关操作
    void clear()
    {
      /**
       * deque的最初状态(无任何元素时)包有一个缓冲区，因此 clear()完成后恢复初始状态，也一样保留一个缓冲区
       */
      // 针对头尾以外的每个缓冲区(它们一定都是饱满的)
      for (auto node = start.node + 1; node < finish.node; ++node)
      {
        data_allocator::destroy(*node, *node + start.buffer_size);
        data_allocator::deallocate(*node, start.buffer_size);
      }
      if (start.node != finish.node)
      {
        // 至少有头尾两个缓冲区
        data_allocator::destroy(start.cur, start.last);
        data_allocator::destroy(finish.cur, finish.last);
        // 释放尾部缓冲区，注意，头缓冲区保留
        data_allocator::deallocate(finish.first, finish.buffer_size);
      }
      else
        data_allocator::destroy(start.cur, finish.cur); // 只有一个缓冲区，仅析构所有元素而不释放缓冲区内存空间
      // 调整状态
      finish = start;
    }
    // 缩减容器容量
    /**
     * 只释放多余的缓冲区。
     * 重新分配内存，以确保 deque 的容量与大小匹配。
     * 更新 start 和 finish 迭代器，以指向新的内存区域。
     */
    void shrink_to_fit() noexcept
    {
      if (empty())
        return; // 如果容器为空，直接返回
      // 计算当前使用的缓冲区数量
      size_type used_buffers = finish.node - start.node + 1;
      if (used_buffers <= 2)
        return; // 如果使用的缓冲区数量已经是最小值，直接返回

      // 计算新的 map 大小
      size_type new_map_size = used_buffers + 2; // 保留一个头部和一个尾部的备用缓冲区
      // 分配新的 map
      map_pointer new_map = map_allocator::allocate(new_map_size);
      map_pointer new_nstart = new_map + new_map_size / 2;
      // 复制当前使用的缓冲区到新的 map
      zfwstl::copy(start.node, finish.node + 1, new_nstart);
      // for (size_type i = 0; i < used_buffers; ++i)
      // {
      //   new_map[i + 1] = map[i + start.node - map];
      // }

      // destroy_buffer(start.node, finish.node + 1); // 释放旧的 map 中的缓冲区
      map_allocator::deallocate(map, map_size); // 释放旧的 map

      // 更新 map 和 map_size
      map = new_map;
      map_size = new_map_size;

      // 更新 start 和 finish
      start.set_node(new_nstart);
      finish.set_node(new_nstart + used_buffers - 1);
    }
    void assign(size_type n, const value_type &value)
    {
      fill_assign(n, value);
    }

    template <class IIter, typename std::enable_if<
                               zfwstl::is_input_iterator<IIter>::value, int>::type = 0>
    void assign(IIter first, IIter last)
    {
      copy_assign(first, last, iterator_category(first));
    }

    void assign(std::initializer_list<value_type> ilist)
    {
      copy_assign(ilist.begin(), ilist.end(), zfwstl::forward_iterator_tag{});
    }

    // emplace_front / emplace_back / emplace
    template <class... Args>
    void emplace_front(Args &&...args)
    {
      {
        if (start.cur != start.first)
        {
          data_allocator::construct(start.cur - 1, zfwstl::forward<Args>(args)...);
          --start.cur;
        }
        else
        {
          reserve_map_at_front();
          // 配置一个新节点(buffer缓冲区)
          *(start.node - 1) = allocate_node();
          start.set_node(start.node - 1);
          start.cur = start.last - 1;
          data_allocator::construct(start.cur, zfwstl::forward<Args>(args)...);
        }
      }
    }
    // NOTE: Args &&...args：这是一个可变参数模板，它允许函数接受任意数量和类型的参数。
    //  && 表示参数通过右值引用传递，这允许完美转发，即保持参数的左值/右值属性
    template <class... Args>
    void emplace_back(Args &&...args)
    {
      if (finish.cur != finish.last - 1)
      {
        data_allocator::construct(finish.cur, zfwstl::forward<Args>(args)...);
        ++finish.cur;
      }
      else
      {
        reserve_map_at_back();
        // 配置一个新节点(buffer缓冲区)
        *(finish.node + 1) = allocate_node();
        data_allocator::construct(finish.cur, zfwstl::forward<Args>(args)...);
        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
      }
    }
    template <class... Args>
    iterator emplace(iterator pos, Args &&...args)
    {
      if (pos.cur == start.cur)
      {
        emplace_front(zfwstl::forward<Args>(args)...);
        return start;
      }
      else if (pos.cur == finish.cur)
      {
        emplace_back(zfwstl::forward<Args>(args)...);
        return finish - 1;
      }
      return insert_aux(pos, zfwstl::forward<Args>(args)...);
    }

    void push_back(const value_type &x)
    {
      if (finish.cur == (finish.last - 1))
        push_back_aux(x);
      else
      {
        // 有剩余备用空间, 直接在备用空间构建元素 即尾端
        data_allocator::construct(finish.cur, x);
        ++finish.cur;
      }
    }
    void push_front(const value_type &x)
    {
      if (start.cur == start.first)
        push_front_aux(x);
      else
      {
        // 有剩余备用空间, 直接在备用空间构建元素
        data_allocator::construct(start.cur - 1, x);
        --start.cur;
      }
    }
    void pop_front()
    {
      if (start.cur == start.last - 1)
        pop_front_aux(); // 第一个缓冲区仅有一个元素
      else
      {
        data_allocator::destroy(start.cur);
        ++start.cur;
      }
    }
    void pop_back()
    {
      if (finish.cur == finish.last)
        pop_back_aux(); // 最后缓冲区buffer没有任何元素，释放工作
      else
      {
        // 最后缓冲区有一个及以上元素
        --finish.cur;
        data_allocator::destroy(finish.cur);
      }
    }

    iterator insert(iterator position, const value_type &x)
    {
      if (position.cur == start.cur)
      {
        push_front(x);
        return start;
      }
      else if (position.cur == finish.cur)
      {
        push_back(x);
        auto tmp = finish;
        --tmp;
        return tmp;
      }
      else
        return insert_aux(position, x);
    }
    iterator insert(iterator position, value_type &&value)
    {
      if (position.cur == start.cur)
      {
        emplace_front(zfwstl::move(value));
        return start;
      }
      else if (position.cur == finish.cur)
      {
        emplace_back(zfwstl::move(value));
        auto tmp = finish;
        --tmp;
        return tmp;
      }
      else
      {
        return insert_aux(position, zfwstl::move(value));
      }
    }
    // 在 position 位置插入 n 个元素
    void insert(iterator position, size_type n, const value_type &value)
    {
      if (position.cur == start.cur)
      {
        require_capacity(n, true);
        auto new_begin = start - n;
        zfwstl::uninitialized_fill_n(new_begin, n, value);
        start = new_begin;
      }
      else if (position.cur == finish.cur)
      {
        require_capacity(n, false);
        auto new_end = finish + n;
        zfwstl::uninitialized_fill_n(finish, n, value);
        finish = new_end;
      }
      else
      {
        // 在中间位置插入n个元素
        fill_insert(position, n, value);
      }
    }
    template <class IIter, typename std::enable_if<
                               zfwstl::is_input_iterator<IIter>::value, int>::type = 0>
    void insert(iterator position, IIter first, IIter last)
    {
      insert_dispatch(position, first, last, iterator_category(first));
    }

    iterator erase(iterator position)
    {
      iterator next = position;
      ++next;
      auto index = position - start;
      if (index < (size() >> 1))
      {
        zfwstl::copy_backward(start, position, next); // 清除点之前元素比较少，移动清除点之前的元素
        pop_front();
      }
      else
      {
        zfwstl::copy(next, finish, position); // 清除点之后元素较少，移动清除点之后元素
        pop_back();
      }
      return start + index;
    }
    iterator erase(iterator first, iterator last)
    {
      if (first == start && last == finish)
      { // 清除区间是整个dedque
        clear();
        return finish;
      }
      else
      {
        auto n = last - first;             // 清除区间长度
        auto elems_before = first - start; // 清除区间前方元素个数
        if (elems_before < (size() - n) / 2)
        {
          // 前方元素较少
          zfwstl::copy_backward(start, first, last); // 向后移动前方元素(覆盖清除区间)
          iterator new_start = start + n;            // 标记deque新起点
          data_allocator::destroy(start.first, new_start.last);
          // 将冗余的缓冲区释放
          for (auto cur = start.node; cur < new_start.node; ++cur)
            data_allocator::deallocate(*cur, start.buffer_size);
          start = new_start;
        }
        else
        {
          // 后方元素较少
          zfwstl::copy(last, finish, first); // 向前移动后方元素(覆盖清除区间)
          iterator new_finish = finish - n;
          data_allocator::destroy(new_finish.first, finish.last);
          // 将冗余的缓冲区释放
          for (auto cur = new_finish.node + 1; cur <= finish.node; ++cur)
            data_allocator::deallocate(*cur, finish.buffer_size);
          finish = new_finish;
        }
        return start + elems_before;
      }
    }

  protected:
    // 配置一个新节点(buffer缓冲区)
    pointer allocate_node()
    {
      auto tmp = data_allocator::allocate(start.buffer_size);
      return tmp;
    }
    void deallocate_node(pointer p)
    {
      data_allocator::deallocate(p, start.buffer_size);
    }
    // NOTE: deque关键函数！配置多个新节点(buffer缓冲区)
    void create_buffer(map_pointer nstart, map_pointer nfinish)
    {
      map_pointer cur;
      try
      {
        for (cur = nstart; cur <= nfinish; ++cur)
        {
          *cur = data_allocator::allocate(start.buffer_size);
        }
      }
      catch (...)
      {
        while (cur != nstart)
        {
          --cur;
          data_allocator::deallocate(*cur, start.buffer_size);
          *cur = nullptr;
        }
        throw;
      }
    }
    // destroy_buffer 函数
    void destroy_buffer(map_pointer nstart, map_pointer nfinish)
    {
      for (map_pointer n = nstart; n <= nfinish; ++n)
      {
        data_allocator::deallocate(*n, start.buffer_size);
        *n = nullptr;
      }
    }
    // NOTE: deque关键函数！ 确保deque有足够的容量来插入新的元素，具体来说，它会根据需要在deque的前端或后端扩展容量
    void require_capacity(size_type n, bool front)
    {
      if (front && (static_cast<size_type>(start.cur - start.first) < n))
      {
        const size_type need_buffer = (n - (start.cur - start.first)) / start.buffer_size + 1;
        if (need_buffer > static_cast<size_type>(start.node - map))
        {
          reserve_map_at_front(need_buffer);
          return;
        }
        create_buffer(start.node - need_buffer, start.node - 1);
      }
      else if (!front && (static_cast<size_type>(finish.last - finish.cur - 1) < n))
      {
        const size_type need_buffer = (n - (finish.last - finish.cur - 1)) / finish.buffer_size + 1;
        if (need_buffer > static_cast<size_type>((map + map_size) - finish.node - 1))
        {
          reserve_map_at_back(need_buffer);
          return;
        }
        create_buffer(finish.node + 1, finish.node + need_buffer);
      }
    }
    void create_map_and_nodes(size_type num_elements)
    {
      // 需要map节点数=(元素个数/每个缓冲区可容纳元素个数)+1
      // 如果刚好整除，会多分配一个节点
      const size_type num_nodes = num_elements / start.buffer_size + 1;
      // 一个map管理节点数最少8个，最多 ="所需节点数 +2(前后各预留一个，扩充时可用) "
      map_size = zfwstl::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE), num_nodes + 2);
      // std::cout << "buffer_size:" << start.buffer_size << std::endl;
      // std::cout << "map_size:" << map_size << std::endl;
      map = map_allocator::allocate(map_size); // 分配内存空间
      // nstart, nfinish指向map所拥有之全部节点的最中央区段
      map_pointer nstart = map + (map_size - num_nodes) / 2;
      map_pointer nfinish = nstart + num_nodes - 1;
      map_pointer cur;
      try
      {
        // 为 map内的每个现用节点配置buffer缓冲区。所有buffers = deque的可用空间(最后一个buffer可能留有备用空间)
        for (cur = nstart; cur <= nfinish; ++cur)
          *cur = allocate_node();
      }
      catch (...)
      {
        //"commit or rollback"
        map_allocator::deallocate(map, map_size);
        map = nullptr;
        map_size = 0;
        throw;
      }
      // 为deque内的两个迭代器 start, finish设置正确内容
      start.set_node(nstart);
      finish.set_node(nfinish);
      start.cur = start.first;
      // 前面所述，若刚好整除，会多分配一个节点
      // 此时cur就会与first指向相同位置
      finish.cur = finish.first + num_elements % finish.buffer_size;
    }
    void fill_initialize(size_type n, const value_type &x)
    {
      create_map_and_nodes(n);
      map_pointer cur;
      if (n != 0)
      {
        for (cur = start.node; cur < finish.node; ++cur)
        {
          zfwstl::uninitialized_fill(*cur, *cur + start.buffer_size, x);
        }
        // NOTE:因为最后一个map中控节点指向的buffer缓冲区可能会有备用空间未使用，不能将这部分初始化
        zfwstl::uninitialized_fill(finish.first, finish.cur, x);
      }
    }
    // copy_init 函数
    template <class IIter>
    void copy_init(IIter first, IIter last, input_iterator_tag)
    {
      const size_type n = zfwstl::distance(first, last);
      create_map_and_nodes(n);
      for (; first != last; ++first)
        emplace_back(*first);
    }
    template <class FIter>
    void copy_init(FIter first, FIter last, forward_iterator_tag)
    {
      const size_type n = zfwstl::distance(first, last);
      create_map_and_nodes(n);
      for (auto cur = start.node; cur < finish.node; ++cur)
      {
        auto next = first;
        zfwstl::advance(next, start.buffer_size);
        zfwstl::uninitialized_copy(first, next, *cur);
        first = next;
      }
      zfwstl::uninitialized_copy(first, last, finish.first);
    }
    // 只有当最后一个缓冲区只剩一个备用元素空间时才会调用
    void push_back_aux(const value_type &x)
    {
      value_type x_copy = x;
      reserve_map_at_back(); // 若符合某种条件则必须重换一个 map
      // 配置一个新节点(buffer缓冲区)
      *(finish.node + 1) = allocate_node();
      try
      {
        data_allocator::construct(finish.cur, x_copy);
        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
      }
      catch (...)
      {
        deallocate_node(*(finish.node + 1));
        throw;
      }
    }
    // 只有当第一个缓冲区没有任何备用元素时才会被调用
    void push_front_aux(const value_type &x)
    {
      value_type x_copy = x;
      reserve_map_at_front(); // 若符合某种条件则必须重换一个 map
      // 配置一个新节点(buffer缓冲区)
      *(start.node - 1) = allocate_node();
      try
      {
        start.set_node(start.node - 1);
        start.cur = start.last - 1;
        data_allocator::construct(start.cur, x_copy);
      }
      catch (...)
      {
        //"commit or rollback"
        start.set_node(start.node + 1);
        start.cur = start.first;
        deallocate_node(*(start.node - 1));
        throw;
      }
    }
    // 只有当 finish.cur==finish.last时才会被调用
    void pop_back_aux()
    {
      deallocate_node(finish.first); // 释放最后一个缓冲区
      finish.set_node(finish.node - 1);
      finish.cur = finish.last - 1;
      data_allocator::destroy(finish.cur);
    }
    // 只有当 start.cur==start.last-1时才会被调用
    void pop_front_aux()
    {
      data_allocator::destroy(start.cur);
      deallocate_node(start.first); // 释放第一个缓冲区
      start.set_node(start.node + 1);
      start.cur = start.first;
    }
    iterator insert_aux(iterator position, const value_type &x)
    {
      auto index = position - start; // 插入点之前元素个数
      value_type x_copy = x;
      if (index < size() / 2)
      {
        // 插入点之前元素较少
        push_front(front()); // 在最前端加入与第一元素同值元素
        iterator front1 = start;
        ++front1;
        iterator front2 = front1;
        ++front2;
        position = start + index;
        iterator position1 = position;
        ++position1;
        zfwstl::copy(front2, position1, front1);
      }
      else
      {
        // 插入点之后元素较少
        push_back(back());
        iterator back1 = finish;
        --back1;
        iterator back2 = back1;
        --back2;
        position = start + index;
        zfwstl::copy_backward(position, back2, back1);
      }
      *position = x_copy;
      return position;
    }
    template <class IIter>
    void insert_dispatch(iterator position, IIter first, IIter last, input_iterator_tag)
    {
      if (last <= first)
        return;
      const size_type n = zfwstl::distance(first, last);
      const size_type elems_before = position - start;
      if (elems_before < (size() / 2))
      {
        require_capacity(n, true);
      }
      else
      {
        require_capacity(n, false);
      }
      position = start + elems_before;
      auto cur = --last;
      for (size_type i = 0; i < n; ++i, --cur)
      {
        insert(position, *cur);
      }
    }
    template <class FIter>
    void insert_dispatch(iterator position, FIter first, FIter last, forward_iterator_tag)
    {
      if (last <= first)
        return;
      const size_type n = zfwstl::distance(first, last);
      if (position.cur == start.cur)
      {
        require_capacity(n, true);
        auto new_begin = start - n;
        try
        {
          zfwstl::uninitialized_copy(first, last, new_begin);
          start = new_begin;
        }
        catch (...)
        {
          if (new_begin.node != start.node)
            for (map_pointer n = new_begin.node; n <= start.node - 1; ++n)
            {
              data_allocator::deallocate(*n, start.buffer_size);
              *n = nullptr;
            }
          throw;
        }
      }
      else if (position.cur == finish.cur)
      {
        require_capacity(n, false);
        auto new_end = finish + n;
        try
        {
          zfwstl::uninitialized_copy(first, last, finish);
          finish = new_end;
        }
        catch (...)
        {
          if (new_end.node != finish.node)
            for (map_pointer n = finish.node + 1; n <= new_end.node; ++n)
            {
              data_allocator::deallocate(*n, finish.buffer_size);
              *n = nullptr;
            }
          throw;
        }
      }
      else
      {
        copy_insert(position, first, last, n);
      }
    }
    // fill_insert 函数
    void fill_insert(iterator position, size_type n, const value_type &value)
    {
      const size_type elems_before = position - start; // 插入位置前元素数量
      const size_type len = size();
      auto value_copy = value;
      if (elems_before < (len / 2)) // 若在容器前半部分插入
      {
        require_capacity(n, true);
        // 原来的迭代器可能会失效
        auto old_begin = start;
        auto new_begin = start - n;
        position = start + elems_before;
        try
        {
          if (elems_before >= n)
          {
            auto start_n = start + n;
            zfwstl::uninitialized_copy(start, start_n, new_begin); // 在新空间的开始位置进行未初始化复制
            start = new_begin;                                     // 更新容器的开始位置
            zfwstl::copy(start_n, position, old_begin);            // 将插入位置前的元素向后移动
            zfwstl::fill(position - n, position, value_copy);
          }
          else
          {

            zfwstl::uninitialized_fill(zfwstl::uninitialized_copy(start, position, new_begin), start, value_copy); // 在新空间的开始位置进行未初始化填充
            start = new_begin;                                                                                     // 更新容器的开始位置
            zfwstl::fill(old_begin, position, value_copy);                                                         // 在插入位置填充新元素
          }
        }
        catch (...)
        {
          if (new_begin.node != start.node)
            for (map_pointer n = new_begin.node; n <= start.node - 1; ++n)
            {
              data_allocator::deallocate(*n, start.buffer_size);
              *n = nullptr;
            }

          throw;
        }
      }
      else
      {
        require_capacity(n, false);
        // 原来的迭代器可能会失效，重新计算插入位置
        auto old_end = finish;
        auto new_end = finish + n;
        const size_type elems_after = len - elems_before;
        position = finish - elems_after;
        try
        {
          if (elems_after > n)
          {
            auto end_n = finish - n;
            zfwstl::uninitialized_copy(end_n, finish, finish);
            finish = new_end;
            zfwstl::copy_backward(position, end_n, old_end);
            zfwstl::fill(position, position + n, value_copy);
          }
          else
          {
            zfwstl::uninitialized_fill(finish, position + n, value_copy);
            zfwstl::uninitialized_copy(position, finish, position + n);
            finish = new_end;
            zfwstl::fill(position, old_end, value_copy);
          }
        }
        catch (...)
        {
          if (new_end.node != finish.node)
            for (map_pointer n = finish.node + 1; n <= new_end.node; ++n)
            {
              data_allocator::deallocate(*n, finish.buffer_size);
              *n = nullptr;
            }
          throw;
        }
      }
    }
    template <class FIter>
    void copy_insert(iterator position, FIter first, FIter last, size_type n)
    {
      const size_type elems_before = position - start;
      auto len = size();
      if (elems_before < (len / 2))
      {
        reserve_map_at_front();
        // 原来的迭代器可能会失效
        auto old_begin = start;
        auto new_begin = start - n;
        position = start + elems_before;
        try
        {
          if (elems_before >= n)
          {
            auto begin_n = start + n;
            zfwstl::uninitialized_copy(start, begin_n, new_begin);
            start = new_begin;
            zfwstl::copy(begin_n, position, old_begin);
            zfwstl::copy(first, last, position - n);
          }
          else
          {
            auto mid = first;
            zfwstl::advance(mid, n - elems_before);
            zfwstl::uninitialized_copy(first, mid,
                                       zfwstl::uninitialized_copy(start, position, new_begin));
            start = new_begin;
            zfwstl::copy(mid, last, old_begin);
          }
        }
        catch (...)
        {
          if (new_begin.node != start.node)
            for (map_pointer n = new_begin.node; n <= start.node - 1; ++n)
            {
              data_allocator::deallocate(*n, start.buffer_size);
              *n = nullptr;
            }
          throw;
        }
      }
      else
      {
        reserve_map_at_back();
        // 原来的迭代器可能会失效
        auto old_end = finish;
        auto new_end = finish + n;
        const auto elems_after = len - elems_before;
        position = finish - elems_after;
        try
        {
          if (elems_after > n)
          {
            auto end_n = finish - n;
            zfwstl::uninitialized_copy(end_n, finish, finish);
            finish = new_end;
            zfwstl::copy_backward(position, end_n, old_end);
            zfwstl::copy(first, last, position);
          }
          else
          {
            auto mid = first;
            zfwstl::advance(mid, elems_after);
            zfwstl::uninitialized_copy(position, finish,
                                       zfwstl::uninitialized_copy(mid, last, finish));
            finish = new_end;
            zfwstl::copy(first, mid, position);
          }
        }
        catch (...)
        {
          if (new_end.node != finish.node)
            for (map_pointer n = finish.node + 1; n <= new_end.node; ++n)
            {
              data_allocator::deallocate(*n, finish.buffer_size);
              *n = nullptr;
            }
          throw;
        }
      }
    }
    void reserve_map_at_back(size_type nodes_to_add = 1)
    {
      // 若 map尾端的节点备用空间不足，则必须重新换一个map(配置更大的，拷贝原来的，释放原来的)
      if (nodes_to_add + 1 > map_size - (finish.node - map))
        reallocate_map(nodes_to_add, false);
    }
    void reserve_map_at_front(size_type nodes_to_add = 1)
    {
      if (nodes_to_add > start.node - map)
        reallocate_map(nodes_to_add, true);
    }
    void reallocate_map(size_type nodes_to_add, bool add_at_front)
    {
      size_type old_num_nodes = finish.node - start.node + 1;
      size_type new_num_nodes = old_num_nodes + nodes_to_add;
      map_pointer new_nstart;
      if (map_size > 2 * new_num_nodes)
      { // 当前的内存分配是新节点数量的两倍大，说明有足够的空间来添加新节点
        // NOTE:将现有元素挪至合适的deque中间，使得两边空闲空间相对平衡
        new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        if (new_nstart < start.node)
          zfwstl::copy(start.node, finish.node + 1, new_nstart);
        else
          zfwstl::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
      }
      else
      {
        size_type new_map_size = map_size + zfwstl::max(map_size, nodes_to_add) + 2;
        // 配置一块空间给新 map
        map_pointer new_map = map_allocator::allocate(new_map_size);
        new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        // 把原 map内容拷贝过来
        zfwstl::copy(start.node, finish.node + 1, new_nstart);
        // 释放原 map
        map_allocator::deallocate(map, map_size);
        // 设定新的 map起始地址与大小
        map = new_map;
        map_size = new_map_size;
      }
      // 重设迭代器start, finish
      start.set_node(new_nstart);
      finish.set_node(new_nstart + old_num_nodes - 1);
    }

    void fill_assign(size_type n, const value_type &value)
    {
      if (n > size())
      {
        zfwstl::fill(begin(), end(), value);
        insert(end(), n - size(), value);
      }
      else
      {
        erase(begin() + n, end());
        zfwstl::fill(begin(), end(), value);
      }
    }
    template <class IIter>
    void copy_assign(IIter first, IIter last, input_iterator_tag)
    {
      auto first1 = begin();
      auto last1 = end();
      for (; first != last && first1 != last1; ++first, ++first1)
      {
        *first1 = *first;
      }
      if (first1 != last1)
      {
        erase(first1, last1);
      }
      else
      {
        insert_dispatch(finish, first, last, input_iterator_tag{});
      }
    }
    template <class FIter>
    void copy_assign(FIter first, FIter last, forward_iterator_tag)
    {
      const size_type len1 = size();
      const size_type len2 = zfwstl::distance(first, last);
      if (len1 < len2)
      {
        auto next = first;
        zfwstl::advance(next, len1);
        zfwstl::copy(first, next, start);
        insert_dispatch(finish, next, last, forward_iterator_tag{});
      }
      else
      {
        erase(zfwstl::copy(first, last, start), finish);
      }
    }
  };

  //========================模板类外重载操作===============
  // 重载 zfwstl 的 swap
  template <class U>
  void swap(deque<U> &lhs, deque<U> &rhs) noexcept
  {
    lhs.swap(rhs);
  }
  template <typename T>
  bool operator==(const deque<T> &lhs, const deque<T> &rhs)
  {
    if (lhs.size() != rhs.size())
      return false;
    auto tmp1 = lhs.begin();
    auto tmp2 = rhs.begin();
    while (tmp1 != lhs.end() && tmp2 != rhs.end())
    {
      if (*tmp1 != *tmp2)
        return false;
      ++tmp1;
      ++tmp2;
    }
    return true;
  }

  template <typename T>
  bool operator!=(const deque<T> &lhs, const deque<T> &rhs)
  {
    return !(lhs == rhs);
  }
}
#endif // !ZFWSTL_DEQUE_H_