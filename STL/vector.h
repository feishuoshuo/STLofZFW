#ifndef ZFWSTL_VECTOR_H_
#define ZFWSTL_VECTOR_H_
/**
 * 容器
 * 迭代器：Random Access iterators
 */
#include <cstddef>                       //for size_t, ptrdiff_t
#include "../src/memory/allocator.h"     //标准空间配置器
#include "../src/exceptdef.h"            //for 宏MYSTL_DEBUG
#include "../src/memory/unintialized.h"  //for uninitialized_fill_n() uninitialized_copy()
#include "../src/algorithms/algorithm.h" //for copy_backward(), fill(), copy()
#include "../src/iterator.h"             // for iterator_category()萃取迭代器类型, distance(), advance()
namespace zfwstl
{
  template <class T /*,class Alloc=alloc*/>
  class vector
  {
  public:
    //  标准空间配置器
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
    // TODO:以后可以添加反向迭代器
    //  typedef mystl::reverse_iterator<iterator> reverse_iterator;
    //  typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

    allocator_type get_allocator() { return data_allocator(); }
    template <class U>
    friend void swap(typename vector<U>::iterator &a, typename vector<U>::iterator &b);

  protected:
    // 配置空间并填满内容
    iterator allocate_and_fill(size_type n, const T &x)
    {
      iterator result = data_allocator::allocate(n);
      zfwstl::uninitialized_fill_n(result, n, x);
      return result;
    }

  private:
    iterator start;          // 正在用的空间头部
    iterator finish;         // 正在用的空间尾部
    iterator end_of_storage; // 储存空间的尾部
  public:
    // 构造、复制、移动、析构函数
    // TODO：需要完善
    vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
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

    // TODO:移动赋值操作符
    // =================vector数据结构的相关操作=====================
    // 迭代器相关操作
    iterator begin()
    {
      return start;
    }
    iterator end() { return finish; }
    const_iterator begin() const { return start; }
    const_iterator end() const { return finish; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    // TODO:可以添加反向迭代器的相关操作

    // 容量相关操作
    size_type size() const { return static_cast<size_type>(end() - begin()); }    // 返回容器中当前元素的数量
    size_type max_size() const { return static_cast<size_type>(-1) / sizeof(T); } // 返回容器可能容纳的最大元素数量的理论上限
    size_type capacity() const { return static_cast<size_type>(end_of_storage - begin()); }
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
    reference operator[](size_type n) const
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
    iterator insert(iterator position, size_type n, const value_type &x)
    {
      if (n == 0)
        return position;
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
        const size_type len = old_size + zfwstl::max(old_size, n);
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
    // TODO:emplace_back, emplace
    void emplace_back();
    void emplace();
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
    void swap(vector<T> &rhs) noexcept
    {
      if (this != &rhs)
      {
        // 直接交换指针即可
        swap(start, rhs.start);
        swap(finish, rhs.finish);
        swap(end_of_storage, rhs.end_of_storage);
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
    // TODO: void assign(std::initializer_list<value_type> il);

  private:
    // 容器初始化分配大小：用户要多少就给多少，不多分
    void init_space(size_type n, const T &value) noexcept
    {
      start = allocate_and_fill(n, value);
      finish = start + n;
      end_of_storage = finish;
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
  // TODO:operator操作运算符重载
  // 重载 zfwstl 的 swap
  template <class T>
  void swap(vector<T> &lhs, vector<T> &rhs)
  {
    lhs.swap(rhs);
  }
}

#endif // !ZFWSTLSTL_VECTOR_H_