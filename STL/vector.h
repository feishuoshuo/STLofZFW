#ifndef ZFWSTL_VECTOR_H_
#define ZFWSTL_VECTOR_H_
/**
 * 容器
 * 迭代器：Random Access iterators
 */
#include <initializer_list>          // for initializer_list
#include <cstddef>                   // for size_t, ptrdiff_t
#include "../src/memory/allocator.h" // 标准空间配置器
// #include "../src/util.h"                 //for forward
#include "../src/exceptdef.h"            // for 宏MYSTL_DEBUG, THROW_LENGTH_ERROR_IF
#include "../src/memory/unintialized.h"  // for uninitialized_fill_n() uninitialized_copy(), uninitialized_move()
#include "../src/algorithms/algorithm.h" // for lexicographical_compare(), equal(), max(), copy_backward(), fill(), copy()
#include "../src/iterator.h"             // for reverse_iterator, iterator_category()萃取迭代器类型, distance(), advance(), is_input_iterator, forward_iterator_tag
namespace zfwstl
{
  template <class T /*,class Alloc=alloc*/>
  class vector
  {
  public:
    //  专属空间配置器
    typedef zfwstl::simple_allocator<T> data_allocator;
    typedef zfwstl::simple_allocator<T> allocator_type;
    // vector的嵌套型别定义
    // NOTE:通过配置器allocator的类型间接定义下面类型，虽然有点复杂，但是更有利于后续的维护，因为只用修改simple_allocator类型即可
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;

    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef zfwstl::reverse_iterator<iterator> reverse_iterator;
    typedef zfwstl::reverse_iterator<const_iterator> const_reverse_iterator;

    allocator_type get_allocator() { return data_allocator(); }
    template <class U>
    friend void swap(vector<U> &lhs, vector<U> &rhs);

  protected:
    // 配置空间并填满内容
    iterator allocate_and_fill(size_type n, const T &x)
    {
      iterator result = data_allocator::allocate(n);
      zfwstl::uninitialized_fill_n(result, n, x);
      return result;
    }

  private:
    //[start, finish)
    iterator start;          // 正在用的空间头部
    iterator finish;         // 正在用的空间尾部
    iterator end_of_storage; // 储存空间的尾部
  public:
    // 默认构造函数 / 初始化列表构造函数
    explicit vector() noexcept : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
    // 参数化构造函数
    vector(size_type n, const T &value)
    {
      std::cout << n << std::endl;
      init_space(n, value);
    }
    vector(int n, const T &value) { init_space(n, value); }
    vector(long n, const T &value) { init_space(n, value); }
    // TAG: explicit 防止编译器使用该构造函数进行隐式类型转换
    // 杜绝 vector<int> v = 10;编译通过
    explicit vector(size_type n) { init_space(n, T()); }
    vector(std::initializer_list<value_type> ilist)
    {
      range_init(ilist.begin(), ilist.end());
    }
    // 拷贝构造
    vector(const vector &rhs)
    {
      range_init(rhs.start, rhs.finish);
    }
    // 移动构造
    vector(vector &&rhs) noexcept : start(rhs.start), finish(rhs.finish), end_of_storage(rhs.end_of_storage)
    {
      rhs.start = nullptr;
      rhs.finish = nullptr;
      rhs.end_of_storage = nullptr;
    }

    // TAG:enable_if 确保只有当Iter是一个输入迭代器时，这个模板构造函数才是可用的
    /**
     * 如果第一个模板参数为 false，则 std::enable_if 不会定义任何东西，这会导致模板的替换失败，但由于 SFINAE，这种失败不会报错
     * ::value 是一个成员，它存储了 is_input_iterator 的编译时计算结果，通常是一个 bool 值
     * 这里的 0 是一个整数，它在这里没有实际的语义作用，只是作为一个占位符，表示这个模板特化的存在
     */
    template <class Iter, typename std::enable_if<zfwstl::is_input_iterator<Iter>::value, int>::type = 0>
    vector(Iter first, Iter last)
    {
      MYSTL_DEBUG(!(last < first));
      range_init(first, last);
    }
    ~vector()
    {
      size_type n = end_of_storage - start;
      data_allocator::destroy(start, finish);    // 销毁对象
      data_allocator::deallocate(start, n);      // 回收内存空间
      start = finish = end_of_storage = nullptr; // 防止野指针
    }
    //=================operator操作运算符重载=====================
    // 复制赋值操作符
    vector<T> &operator=(const vector &rhs)
    {
      if (this != &rhs)
      {
        const auto len = rhs.size();
        if (len > capacity())
        {
          vector tmp(rhs.begin(), rhs.end());
          swap(tmp);
        }
        else if (size() >= len)
        {
          auto i = zfwstl::copy(rhs.begin(), rhs.end(), begin());
          data_allocator::destroy(i, finish);
          finish = start + len;
        }
        else
        {
          zfwstl::copy(rhs.begin(), rhs.begin() + size(), start);
          zfwstl::uninitialized_copy(rhs.begin() + size(), rhs.end(), finish);
          end_of_storage = finish = start + len;
        }
      }
      return *this;
    }
    vector &operator=(vector &&rhs) noexcept
    {
      data_allocator::destroy(start, finish);
      data_allocator::deallocate(start, end_of_storage - start);
      start = rhs.start;
      finish = rhs.finish;
      end_of_storage = rhs.end_of_storage;

      rhs.start = nullptr;
      rhs.finish = nullptr;
      rhs.end_of_storage = nullptr;
      return *this;
    }
    vector &operator=(std::initializer_list<value_type> ilist)
    {
      vector tmp(ilist.begin(), ilist.end());
      swap(tmp);
      return *this;
    }
    // =================vector数据结构的相关操作=====================
    // 迭代器相关操作
    iterator begin() { return start; }
    iterator end() { return finish; }
    const_iterator begin() const { return start; }
    const_iterator end() const { return finish; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const { return rend(); }

    // 容量相关操作
    size_type size() const { return static_cast<size_type>(end() - begin()); }    // 返回容器中当前元素的数量
    size_type max_size() const { return static_cast<size_type>(-1) / sizeof(T); } // 返回容器可能容纳的最大元素数量的理论上限
    size_type capacity() const { return static_cast<size_type>(end_of_storage - begin()); }
    // 调整容量以至少能够容纳 n 个元素
    void reserve(size_type n)
    {
      if (capacity() < n) // 会导致容器重新分配其存储空间，以增加其容量至 n 或更大
      {
        THROW_LENGTH_ERROR_IF(n > max_size(),
                              "n can not larger than max_size() in vector<T>::reserve(n)");
        const auto old_size = size();
        auto tmp = data_allocator::allocate(n);
        zfwstl::uninitialized_move(start, finish, tmp);
        data_allocator::deallocate(start, end_of_storage - start);
        start = tmp;
        finish = tmp + old_size;
        end_of_storage = start + n;
      }
    }
    bool empty() const { return (begin() == end()); }
    // 访问元素相关操作
    reference front()
    {
      MYSTL_DEBUG(!empty());
      return *begin();
    }
    reference front() const
    {
      MYSTL_DEBUG(!empty());
      return *begin();
    }
    reference back()
    {
      MYSTL_DEBUG(!empty());
      return *(end() - 1); //-1 因为finish指针指向尾部不是最后一个元素！！
    }
    reference back() const
    {
      MYSTL_DEBUG(!empty());
      return *(end() - 1);
    }
    reference operator[](size_type n)
    {
      MYSTL_DEBUG(n < size());
      return *(begin() + n);
    }
    const_reference operator[](size_type n) const
    {
      MYSTL_DEBUG(n < size());
      return *(begin() + n);
    }
    pointer data() noexcept { return start; }
    const_pointer data() const noexcept { return start; }
    // 用于访问向量（vector）中的元素，同时提供边界检查(与operator[]最大不同之处)
    reference at(size_type n)
    {
      THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
      return (*this)[n];
    }
    const_reference at(size_type n) const
    {
      THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
      return (*this)[n];
    }
    // 修改容器相关操作
    // 尾部插入元素
    void push_back(const T &x)
    {
      if (finish != end_of_storage)
      {
        data_allocator::construct(finish, x);
        ++finish;
      }
      else
      {
        // 无备份空间->扩容
        inser_aux(end(), x);
      }
    }
    // 指定位置插入 n 个元素，元素初值 = x
    void insert(iterator position, size_type n, const value_type &x)
    {
      if (n == 0)
        return;
      if (size_type(end_of_storage - finish) >= n)
      {
        // 1-备用空间 >= 新增元素个数
        const value_type x_copy = x;                     // 避免被覆盖
        const size_type elems_after = finish - position; // 计算插入点之后现有元素个数
        iterator old_finish = finish;
        if (elems_after > n)
        {
          // 1-1插入点之后现有元素个数 > 新增元素个数
          zfwstl::uninitialized_copy(finish - n, finish, old_finish);
          finish += n;
          zfwstl::copy_backward(position, old_finish - n, old_finish);
          zfwstl::fill(position, position + n, x_copy);
        }
        else
        {
          // 1-2插入点之后现有元素个数 <= 新增元素个数
          zfwstl::uninitialized_fill_n(finish, n - elems_after, x_copy);
          finish += n - elems_after;
          zfwstl::uninitialized_copy(position, old_finish, finish);
          finish += elems_after;
          zfwstl::fill(position, old_finish, x_copy);
        }
      }
      else
      {
        // 2-备用空间 < 新增元素个数 (需要配置额外内存)
        const size_type old_size = size();
        const size_type len = old_size + zfwstl::max(old_size, n); // 扩容关键
        // 配置新的vector空间
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        try
        {
          new_finish = zfwstl::uninitialized_copy(start, position, new_start);
          new_finish = zfwstl::uninitialized_fill_n(new_finish, n, x);
          new_finish = zfwstl::uninitialized_copy(position, finish, new_finish);
        }
        catch (...)
        {
          // 如果有异常，则 "commit or rollback"
          data_allocator::destroy(new_start, new_finish);
          data_allocator::deallocate(new_start, len);
          throw;
        }
        // 清楚并释放旧的vector
        data_allocator::destroy(start, finish);
        data_allocator::deallocate(start, finish - start);
        // 调整水位
        start = new_start;
        finish = new_finish;
        end_of_storage = start + len;
      }
    }
    void insert(iterator position, const value_type &x)
    {
      insert(position, 1, x);
    }
    // emplace_back, emplace
    // 在 pos 位置就地构造元素，避免额外的复制或移动开销
    template <class... Args>
    iterator emplace(const_iterator pos, Args &&...args)
    {
      MYSTL_DEBUG(pos >= begin() && pos <= end());
      iterator xpos = const_cast<iterator>(pos);
      const size_type n = xpos - start;
      if (finish != end_of_storage && xpos == finish)
      {
        // TAG: zfwstl::forward<Args>(args)...用于将参数包 args 完美转发给 construct 函数，保留每个参数的左值或右值属性
        data_allocator::construct(finish, zfwstl::forward<Args>(args)...);
        ++finish;
      }
      else if (finish != end_of_storage) // 需要其他元素挪动位置
      {
        auto new_end = finish;
        data_allocator::construct(finish, *(finish - 1));
        ++new_end;
        zfwstl::copy_backward(xpos, finish - 1, finish);
        *xpos = value_type(zfwstl::forward<Args>(args)...);
        finish = new_end;
      }
      else // 需要扩容
      {
        emplace_aux(xpos, zfwstl::forward<Args>(args)...);
      }
      return begin() + n;
    }
    template <class... Args>
    void emplace_back(Args &&...args)
    {
      if (finish < end_of_storage)
      {
        data_allocator::construct(finish, zfwstl::forward<Args>(args)...);
        ++finish;
      }
      else
        emplace_aux(finish, zfwstl::forward<Args>(args)...);
    }
    // 尾部取出元素
    void pop_back()
    {
      MYSTL_DEBUG(!empty());
      data_allocator::destroy(finish - 1);
      --finish;
    }
    // 删除[first, last)范围上的元素
    iterator erase(iterator first, iterator last)
    {
      MYSTL_DEBUG(first >= begin() && last <= end() && !(last < first));
      iterator i = zfwstl::copy(last, finish, first);
      data_allocator::destroy(i, finish);
      finish = finish - (last - first);
      return first;
    }
    // 指定位置删除元素
    iterator erase(iterator position)
    {
      MYSTL_DEBUG(position >= begin() && position < end());
      if (position + 1 != end()) // 判断位置不是最后一个元素，即中间元素需要移位
        zfwstl::copy(position + 1, finish, position);
      --finish;
      data_allocator::destroy(finish); // 不用deallocate释放内存，只用析构掉对象即可
      return position;
    }
    // 清空容器
    void clear() { erase(begin(), end()); }
    // 与另一个 vector 交换
    void swap(vector<T> &rhs)
    {
      if (this != &rhs)
      {
        // 直接交换指针即可
        zfwstl::swap(start, rhs.start);
        zfwstl::swap(finish, rhs.finish);
        zfwstl::swap(end_of_storage, rhs.end_of_storage);
      }
    }
    // 重置容器大小
    void resize(size_type new_size) { return resize(new_size, value_type()); }
    void resize(size_type new_size, const value_type &value)
    {
      if (new_size < size())
        erase(begin() + new_size, end());
      else
        insert(end(), new_size - size(), value);
    }

    // 覆盖替换操作 assign
    template <class Iter, typename std::enable_if<
                              zfwstl::is_input_iterator<Iter>::value, int>::type = 0>
    void assign(Iter first, Iter last)
    {
      MYSTL_DEBUG(!(last < first));
      copy_assign(first, last, zfwstl::iterator_category(first));
    }
    // 从头覆盖 n个重复的 value值
    void assign(size_type n, const value_type &value)
    {
      if (n > capacity()) // 超过容器容纳容量
      {
        vector tmp(n, value);
        swap(tmp);
      }
      else if (n > size()) // 超过容器实际元素个数
      {
        // 覆盖剩余元素
        zfwstl::fill(begin(), end(), value);
        // 创建余下超出的元素
        finish = zfwstl::uninitialized_fill_n(finish, n - size(), value);
      }
      else
      {
        erase(zfwstl::fill_n(start, n, value), finish);
      }
    }
    void assign(std::initializer_list<value_type> il)
    {
      copy_assign(il.begin(), il.end(), zfwstl::forward_iterator_tag{});
    }

  private:
    // 容器初始化分配大小：用户要多少就给多少，不多分
    void init_space(size_type n, const T &value) noexcept
    {
      start = allocate_and_fill(n, value);
      finish = start + n;
      end_of_storage = finish;
    }
    // range_init 函数
    template <class Iter>
    void range_init(Iter first, Iter last)
    {
      const size_type len = zfwstl::distance(first, last);
      const size_type init_size = zfwstl::max(len, static_cast<size_type>(16));
      init_space(len, init_size);
      zfwstl::uninitialized_copy(first, last, start);
    }
    // 指定位置插入元素，涉及是否需要扩容
    void inser_aux(iterator position, const T &x)
    {
      if (finish != end_of_storage) // 还有备份空间
      {
        data_allocator::construct(finish, *(finish - 1));
        // 调整水位
        ++finish;
        value_type x_copy = x;
        zfwstl::copy_backward(position, finish - 2, finish - 1); // 从后往前复制
        *position = x_copy;
      }
      else
      {
        // 没有备份空间-->扩容
        const size_type old_size = size();
        const size_type len = old_size != 0 ? 2 * old_size : 1; // 扩容原来的 2 倍
        // 前半段放原数据，后半段用来放新数据
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        try
        {
          // 将原vector内容拷贝到新vector
          new_finish = zfwstl::uninitialized_copy(start, position, new_start);
          data_allocator::construct(new_finish, x); // 为新元素设定初值 x
          // 调整水位
          ++new_finish;
          // new_finish = zfwstl::uninitialized_copy(position, finish, new_finish);//将原vector的备用空间中的内容也忠实地拷贝过来
        }
        catch (...) //... 捕获所有异常的异常处理块
        {
          // commit or rollback
          data_allocator::destroy(new_start, new_finish);
          data_allocator::deallocate(new_start, len);
          throw;
        }
        // 释放掉原内存空间
        data_allocator::destroy(begin(), end());
        data_allocator::deallocate(start, size());
        // 调整迭代器，指向新vector
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
      }
    }
    // 就地构造元素，涉及是否需要扩容
    template <class... Args>
    void emplace_aux(iterator position, Args &&...args)
    {
      if (finish != end_of_storage) // 还有备份空间
      {
        data_allocator::construct(finish, *(finish - 1));
        // 调整水位
        ++finish;
        value_type x_copy = value_type(zfwstl::forward<Args>(args)...);
        zfwstl::copy_backward(position, finish - 2, finish - 1); // 从后往前复制
        *position = zfwstl::move(x_copy);
      }
      else
      {
        // 没有备份空间-->扩容
        const size_type old_size = size();
        const size_type len = old_size != 0 ? 2 * old_size : 1; // 扩容原来的 2 倍
        // 前半段放原数据，后半段用来放新数据
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        try
        {
          // 将原vector内容拷贝到新vector
          new_finish = zfwstl::uninitialized_copy(start, position, new_start);
          data_allocator::construct(new_finish, zfwstl::forward<Args>(args)...); // 为新元素设定初值 x
          // 调整水位
          ++new_finish;
          // new_finish = zfwstl::uninitialized_copy(position, finish, new_finish);//将原vector的备用空间中的内容也忠实地拷贝过来
        }
        catch (...) //... 捕获所有异常的异常处理块
        {
          // commit or rollback
          data_allocator::destroy(new_start, new_finish);
          data_allocator::deallocate(new_start, len);
          throw;
        }
        // 释放掉原内存空间
        data_allocator::destroy(begin(), end());
        data_allocator::deallocate(start, size());
        // 调整迭代器，指向新vector
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
      }
    }
    template <class IIter>
    void copy_assign(IIter first, IIter last, input_iterator_tag)
    {
      auto cur = start;
      for (; first != last && finish != start; ++first, ++cur)
        *cur = *first;
      if (first == last)
        erase(cur, finish);
      else
        insert(finish, first, last);
    }
    template <class FIter>
    void copy_assign(FIter first, FIter last, forward_iterator_tag)
    {
      const size_type len = zfwstl::distance(first, last);
      if (len > capacity())
      {
        vector tmp(first, last);
        swap(tmp);
      }
      else if (size() >= len)
      {
        auto new_finish = zfwstl::copy(first, last, start);
        data_allocator::destroy(new_finish, finish);
        finish = new_finish;
      }
      else
      {
        auto mid = first;
        zfwstl::advance(mid, size());
        zfwstl::copy(first, mid, start);
        auto new_finish = zfwstl::uninitialized_copy(mid, last, finish);
        finish = new_finish;
      }
    }
  };

  //========================模板类外重载操作===============
  // 重载 zfwstl 的 swap
  template <class T>
  void swap(vector<T> &lhs, vector<T> &rhs)
  {
    lhs.swap(rhs);
  }
  template <class T>
  bool operator==(const vector<T> &lhs, const vector<T> &rhs)
  {
    return lhs.size() == rhs.size() &&
           zfwstl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }
  template <typename T>
  bool operator!=(const vector<T> &lhs, const vector<T> &rhs)
  {
    return !(lhs == rhs);
  }
  template <class T>
  bool operator<(const vector<T> &lhs, const vector<T> &rhs)
  {
    return zfwstl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }
  template <class T>
  bool operator>(const vector<T> &lhs, const vector<T> &rhs)
  {
    return rhs < lhs;
  }
  template <class T>
  bool operator<=(const vector<T> &lhs, const vector<T> &rhs)
  {
    return !(rhs < lhs);
  }
  template <class T>
  bool operator>=(const vector<T> &lhs, const vector<T> &rhs)
  {
    return !(lhs < rhs);
  }
}

#endif // !ZFWSTLSTL_VECTOR_H_