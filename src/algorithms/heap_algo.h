#ifndef ZFWSTL_HEAP_H_
#define ZFWSTL_HEAP_H_
/**
 * heap(隐式表述，implicit representation)
 * 不归属于STL容器组件，幕后英雄，扮演priority queue的助手；一种算法
 * 底层数据结构：使用array实现完全二叉树(complete binary tree)
 * heap 的四个算法: push_heap, pop_heap, sort_heap, make_heap
 */
#include <cstddef>       //for size_t, ptrdiff_t
#include "../iterator.h" //for distance_type
namespace zfwstl
{

  // =====================================push_heap=================================
  // 该函数接受两个迭代器，表示一个 heap 容器的首尾，并且新元素已经插入到底部容器的最尾端，调整 heap
  // 大顶堆heap 上层元素均大于下层
  // holeIndex, topIndex均为相对距离
  template <class RandomIter, class Distance, class T>
  void __push_heap(RandomIter first, Distance holeIndex, Distance topIndex, T value)
  {
    auto parent = (holeIndex - 1) / 2; // 找出父节点
    // 未到达顶端，且父节点的值小于新值
    while (holeIndex > topIndex && *(first + parent) < value)
    {
      // 使用 operator<，所以 heap 为 max-heap
      *(first + holeIndex) = *(first + parent);
      holeIndex = parent;
      parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
  }
  // 重载版本使用函数对象 comp 代替比较操作
  template <class RandomIter, class Distance, class T, class Compared>
  void __push_heap(RandomIter first, Distance holeIndex, Distance topIndex, T value,
                   Compared comp)
  {
    auto parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value))
    {
      *(first + holeIndex) = *(first + parent);
      holeIndex = parent;
      parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
  }

  template <class RandomIter, class Distance, class T>
  void __push_heap_aux(RandomIter first, RandomIter last, Distance *, T *)
  { // 容器最尾端，即第一个洞号：(last-first)-1
    zfwstl::__push_heap(first, Distance(last - first) - 1, static_cast<Distance>(0), T(*(last - 1)));
  }
  template <class RandomIter, class Distance, class T, class Compared>
  void __push_heap_aux(RandomIter first, RandomIter last, Distance *, T *, Compared comp)
  { // 容器最尾端，即第一个洞号：(last-first)-1
    zfwstl::__push_heap(first, Distance(last - first) - 1, static_cast<Distance>(0), T(*(last - 1)), comp);
  }

  template <class RandomIter>
  void push_heap(RandomIter first, RandomIter last)
  { // 新元素应该已置于底部容器的最尾端
    zfwstl::__push_heap_aux(first, last, distance_type(first), value_type(first));
  }
  template <class RandomIter, class Compared>
  void push_heap(RandomIter first, RandomIter last, Compared comp)
  { // 新元素应该已置于底部容器的最尾端
    zfwstl::__push_heap_aux(first, last, distance_type(first), value_type(first), comp);
  }
  // =====================================pop_heap=================================
  // 该函数接受两个迭代器，表示 heap 容器的首尾，将 heap 的根节点取出放到容器尾部，调整 heap
  template <class RandomIter, class T, class Distance>
  void __adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
  { // 先进行下溯(percolate down)过程
    auto topIndex = holeIndex;
    auto rchild = 2 * holeIndex + 2;
    while (rchild < len)
    {                                                // 下溯找到下层较大的子节点
      if (*(first + rchild) < *(first + rchild - 1)) // 默认右子节点较大，但是若左子节点大于右子节点，则rchild指向左子节点
        --rchild;
      *(first + holeIndex) = *(first + rchild);
      holeIndex = rchild;
      rchild = 2 * (rchild + 1);
    }
    if (rchild == len)
    { // 如果没有右子节点
      *(first + holeIndex) = *(first + (rchild - 1));
      holeIndex = rchild - 1;
    }
    // 再执行一次上溯(percolate up)过程
    zfwstl::__push_heap(first, holeIndex, topIndex, value);
  }
  // 重载版本使用函数对象 comp 代替比较操作
  template <class RandomIter, class T, class Distance, class Compared>
  void __adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,
                     Compared comp)
  {
    // 先进行下溯(percolate down)过程
    auto topIndex = holeIndex;
    auto rchild = 2 * holeIndex + 2;
    while (rchild < len)
    {
      if (comp(*(first + rchild), *(first + rchild - 1)))
        --rchild;
      *(first + holeIndex) = *(first + rchild);
      holeIndex = rchild;
      rchild = 2 * (rchild + 1);
    }
    if (rchild == len)
    {
      *(first + holeIndex) = *(first + (rchild - 1));
      holeIndex = rchild - 1;
    }
    // 再执行一次上溯(percolate up)过程
    zfwstl::__push_heap(first, holeIndex, topIndex, value, comp);
  }

  template <class RandomIter, class T, class Distance>
  void adjust_heap(RandomIter first, RandomIter last, RandomIter result, T value, Distance *)
  {
    *result = *first;
    __adjust_heap(first, static_cast<Distance>(0), Distance(last - first), value);
  }
  template <class RandomIter, class T, class Distance, class Compared>
  void adjust_heap(RandomIter first, RandomIter last, RandomIter result, T value, Distance *, Compared comp)
  {
    *result = *first;
    __adjust_heap(first, static_cast<Distance>(0), Distance(last - first), value, comp);
  }

  template <class RandomIter, class T, class Compared>
  void __pop_heap_aux(RandomIter first, RandomIter last, T *, Compared comp)
  {
    zfwstl::adjust_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first), comp);
  }
  template <class RandomIter, class T>
  void __pop_heap_aux(RandomIter first, RandomIter last, T *)
  {
    zfwstl::adjust_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
    // pop操作应该是弹出顶堆元素，即array第一个元素
    // 1.设定欲调整值为尾值
    // 2.将首值调至尾节点(所以将迭代器result设为last-1)
    // 3.重整[first, last-1)，使之成为一个合格的顶堆heap
  }

  template <class RandomIter>
  void pop_heap(RandomIter first, RandomIter last)
  {
    zfwstl::__pop_heap_aux(first, last, value_type(first));
  }
  template <class RandomIter, class Compared>
  void pop_heap(RandomIter first, RandomIter last, Compared comp)
  {
    zfwstl::__pop_heap_aux(first, last, value_type(first), comp);
  }

  // =====================================sort_heap=================================
  // 该函数接受两个迭代器，表示 heap 容器的首尾，不断执行 pop_heap 操作，直到首尾最多相差1
  template <class RandomIter>
  void sort_heap(RandomIter first, RandomIter last)
  {
    // 每执行一次 pop_heap，最大的元素都被放到尾部，直到容器最多只有一个元素，完成排序
    while (last - first > 1)
    {
      zfwstl::pop_heap(first, last--); // 每执行一次pop，操作范围即缩退一个
    }
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class RandomIter, class Compared>
  void sort_heap(RandomIter first, RandomIter last, Compared comp)
  {
    while (last - first > 1)
    {
      zfwstl::pop_heap(first, last--, comp);
    }
  }
  // =====================================make_heap=================================
  // 该函数接受两个迭代器，表示 heap 容器的首尾，把容器内的数据变为一个 heap
  template <class RandomIter, class Distance>
  void __make_heap(RandomIter first, RandomIter last, Distance *)
  {
    if (last - first < 2)
      return;
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;
    while (true)
    {
      // 重排以 holeIndex 为首的子树
      zfwstl::__adjust_heap(first, holeIndex, len, *(first + holeIndex));
      if (holeIndex == 0)
        return;
      holeIndex--;
    }
  }
  // 重载版本使用函数对象 comp 代替比较操作
  template <class RandomIter, class Distance, class Compared>
  void __make_heap(RandomIter first, RandomIter last, Distance *, Compared comp)
  {
    if (last - first < 2)
      return;
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;
    while (true)
    {
      // 重排以 holeIndex 为首的子树
      zfwstl::__adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
      if (holeIndex == 0)
        return;
      holeIndex--;
    }
  }

  template <class RandomIter, class Compared>
  void make_heap(RandomIter first, RandomIter last, Compared comp)
  {
    zfwstl::__make_heap(first, last, distance_type(first), comp);
  }
  template <class RandomIter>
  void make_heap(RandomIter first, RandomIter last)
  {
    zfwstl::__make_heap(first, last, distance_type(first));
    ;
  }
}

#endif // !ZFWSTL_HEAP_H_