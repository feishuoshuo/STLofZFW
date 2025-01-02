#ifndef ZFWSTL_ALGO_H_
#define ZFWSTL_ALGO_H_
#include <type_traits> // for enable_if()
#include <cstddef>
#include <ctime>           // for time()
#include <cstring>         // for memmove()
#include "../iterator.h"   // for XXX_iterator_tag, iterator_traits, reverse_iterator, distance(), advance()
#include "../util.h"       // for pair, swap()
#include "../functional.h" // for equal_to
#include "algobase.h"      // for iter_swap()
#include "heap_algo.h"     // forpush_heap, pop_heap, sort_heap, make_heap
#include <memory>          // for temporary_buffer
/**
 * all_of、any_of、none_of
 * median 找出三个值的中间值
 * adjacent_find 查找相邻而重复(或符合某条件)的元素
 * binary_search 二分查找,是否有等同元素
 * count 计数
 * count_if 在特定条件下计数
 * equal_range 试图在有序区间中寻找某值(返回一个上下限区间)
 * find 循序查找
 * find_if 循序查找符合特定条件者(首次出现的)
 * find_if_not 不符条件(首次出现的)
 * find_end 查找某个子序列的最后一次出现点(最后出现的)
 * find_first_of 查找某些元素首次出现点
 * for_each 对区间内的每一个元素实施某操作
 * generate 以特定操作之运算结果填充特定区间内的元素
 * generate_n 以特定操作之运算结果填充 n 个元素内容
 * includes 是否涵盖于某序列之中(应用于有序区间)
 * inplace_merge 合并并就地替换(覆盖上去)(应用于有序区间)
 * is_heap * 判断某区间是否为一个heap
 * is_sorted * 判断某区间是否已排序
 * lower_bound 将特定元素插入区间之内而不影响区间之原本排序(应用于有序区间)
 * max_element 最大值所在位置
 * merge 合并两个序列(应用于有序区间)
 * merge_sort 归并排序
 * min_element 最小值所在位置
 * next_permutation 获得下一个排列组合
 * is_permutation
 * nth_element 重新安排序列中的第n个元素的左右两端
 * partial_sort 局部排序
 * partial_sort_copy 局部求和
 * partition 分割
 * partition_copy 将分割的结果放置新的两个区间
 * prev_permutation 获得前一个排列组合
 * random_shuffle 随机重排元素
 * random_sample * 随机取样
 * random_sample_n *
 * remove 移除(但不删除)
 * remove_copy
 * remove_if
 * remove_copy_if
 * replace 替换
 * replace_copy
 * replace_if
 * replace_copy_if
 * reverse 反转
 * reverse_copy
 * rotate 旋转
 * rotate_copy
 * search 查找某个子序列
 * search_n 查找“连续发生n次”的子序列
 * sort 排序
 * stable_partition 分割并保持元素的相对次序(对应区别partition)
 * stable_sort 排序并保持等值元素的相对次序
 * swap_ranges 交换指定区间
 * transform 以两个区间为基础，交换作用产生在第三个区间
 * unique
 * unique_copy 将重复元素复制到他处
 * upper_bound “将指定元素插入区间之内而不影响区间原本排序”的最高位置(应用于有序区间)
 * ================================================================
 * move 区间移动
 */
namespace zfwstl
{
  template <class ForwardIter, class T>
  ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T &value);
  template <class ForwardIter, class T>
  ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T &value);
  template <class ForwardIter1, class ForwardIter2>
  ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2);
  template <class ForwardIter>
  void rotate(ForwardIter, ForwardIter, ForwardIter);
  template <class InputIter1, class InputIter2, class OutputIter>
  OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result);
  template <class BidirectionalIter>
  void reverse(BidirectionalIter first, BidirectionalIter last);
  template <class BidirectionalIter>
  void reverse(BidirectionalIter first, BidirectionalIter last);
  template <class RandomIter, class T>
  RandomIter unchecked_partition(RandomIter first, RandomIter last, const T &pivot);
  template <class RandomIter>
  void insertion_sort(RandomIter first, RandomIter last);
  template <class InputIter, class OutputIter, class T>
  OutputIter remove_copy(InputIter first, InputIter last, OutputIter result, const T &value);

  // ===========================all_of===========================
  // 检查[first, last)内是否全部元素都满足一元操作 unary_pred 为 true 的情况，满足则返回 true
  template <class InputIter, class UnaryPredicate>
  bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
  {
    for (; first != last; ++first)
    {
      if (!unary_pred(*first))
        return false;
    }
    return true;
  }

  // ===========================any_of===========================
  // 检查[first, last)内是否存在某个元素满足一元操作 unary_pred 为 true 的情况，满足则返回 true
  template <class InputIter, class UnaryPredicate>
  bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
  {
    for (; first != last; ++first)
    {
      if (unary_pred(*first))
        return true;
    }
    return false;
  }

  // ===========================none_of===========================
  // 检查[first, last)内是否全部元素都不满足一元操作 unary_pred 为 true 的情况，满足则返回 true
  template <class InputIter, class UnaryPredicate>
  bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
  {
    for (; first != last; ++first)
    {
      if (unary_pred(*first))
        return false;
    }
    return true;
  }

  // ===========================median===========================
  // 找出三个值的中间值
  template <class T>
  const T &median(const T &left, const T &mid, const T &right)
  {
    if (left < mid)
      if (mid < right) // left < mid < right
        return mid;
      else if (left < right) // left < right <= mid
        return right;
      else // right <= left < mid
        return left;
    else if (left < right) // mid <= left < right
      return left;
    else if (mid < right) // mid < right <= left
      return right;
    else // right <= mid <= left
      return mid;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class T, class Compared>
  const T &median(const T &left, const T &mid, const T &right, Compared comp)
  {
    if (comp(left, mid))
      if (comp(mid, right))
        return mid;
      else if (comp(left, right))
        return right;
      else
        return left;
    else if (comp(left, right))
      return left;
    else if (comp(mid, right))
      return right;
    else
      return mid;
  }

  // ===========================adjacent_find===========================
  // 找出第一对匹配的相邻元素，缺省使用 operator== 比较，如果找到返回一个迭代器，指向这对元素的第一个元素
  template <class ForwardIter>
  ForwardIter adjacent_find(ForwardIter first, ForwardIter last)
  {
    if (first == last)
      return last;
    auto next = first;
    while (++next != last)
    {
      if (*first == *next)
        return first;
      first = next;
    }
    return last;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class ForwardIter, class Compared>
  ForwardIter adjacent_find(ForwardIter first, ForwardIter last, Compared comp)
  {
    if (first == last)
      return last;
    auto next = first;
    while (++next != last)
    {
      if (comp(*first, *next))
        return first;
      first = next;
    }
    return last;
  }

  // ===========================binary_search===========================
  // 二分查找，如果在[first, last)内有等同于 value 的元素，返回 true，否则返回 false
  template <class ForwardIter, class T>
  bool binary_search(ForwardIter first, ForwardIter last, const T &value)
  {
    auto i = zfwstl::lower_bound(first, last, value); // lower_bound来实现二分查找
    return i != last && !(value < *i);
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class ForwardIter, class T, class Compared>
  bool binary_search(ForwardIter first, ForwardIter last, const T &value, Compared comp)
  {
    auto i = zfwstl::lower_bound(first, last, value);
    return i != last && !comp(value, *i);
  }

  // ===========================count===========================
  // 对[first, last)区间内的元素与给定值进行比较，缺省使用 operator==，返回元素相等的个数
  template <class InputIter, class T>
  typename zfwstl::iterator_traits<InputIter>::difference_type
  count(InputIter first, InputIter last, const T &value)
  {
    typename zfwstl::iterator_traits<InputIter>::difference_type n = 0;
    for (; first != last; ++first)
    {
      if (*first == value)
        ++n;
    }
    return n;
  }

  // ===========================count_if===========================
  // 对[first, last)区间内的每个元素都进行一元 unary_pred 操作，返回结果为 true 的个数
  template <class InputIter, class UnaryPredicate>
  typename zfwstl::iterator_traits<InputIter>::difference_type
  count_if(InputIter first, InputIter last, UnaryPredicate unary_pred)
  {
    typename zfwstl::iterator_traits<InputIter>::difference_type n = 0;
    for (; first != last; ++first)
    {
      if (unary_pred(*first))
        ++n;
    }
    return n;
  }

  // ===========================equal_range===========================
  /**
   * 二分查找[first,last)有序区间中与 value 相等的元素所形成的区间，返回一对迭代器指向区间首尾
   * 第一个迭代器指向第一个不小于 value 的元素，第二个迭代器指向第一个大于 value 的元素
   */
  // erange_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter, class T>
  zfwstl::pair<ForwardIter, ForwardIter>
  erange_dispatch(ForwardIter first, ForwardIter last,
                  const T &value, forward_iterator_tag)
  {
    auto len = zfwstl::distance(first, last);
    auto half = len;
    ForwardIter middle, left, right;
    while (len > 0)
    {
      half = len >> 1;
      middle = first;
      zfwstl::advance(middle, half);
      if (*middle < value)
      {
        first = middle;
        ++first;
        len = len - half - 1;
      }
      else if (value < *middle)
      {
        len = half;
      }
      else
      {
        left = zfwstl::lower_bound(first, last, value);
        zfwstl::advance(first, len);
        right = zfwstl::upper_bound(++middle, first, value);
        return zfwstl::pair<ForwardIter, ForwardIter>(left, right);
      }
    }
    return zfwstl::pair<ForwardIter, ForwardIter>(last, last);
  }

  // erange_dispatch 的 random_access_iterator_tag 版本
  template <class RandomIter, class T>
  zfwstl::pair<RandomIter, RandomIter>
  erange_dispatch(RandomIter first, RandomIter last,
                  const T &value, random_access_iterator_tag)
  {
    auto len = last - first;
    auto half = len;
    RandomIter middle, left, right;
    while (len > 0)
    {
      half = len >> 1;
      middle = first + half;
      if (*middle < value)
      {
        first = middle + 1;
        len = len - half - 1;
      }
      else if (value < *middle)
      {
        len = half;
      }
      else
      {
        left = zfwstl::lower_bound(first, middle, value);
        right = zfwstl::upper_bound(++middle, first + len, value);
        return zfwstl::pair<RandomIter, RandomIter>(left, right);
      }
    }
    return zfwstl::pair<RandomIter, RandomIter>(last, last);
  }

  template <class ForwardIter, class T>
  zfwstl::pair<ForwardIter, ForwardIter>
  equal_range(ForwardIter first, ForwardIter last, const T &value)
  {
    return zfwstl::erange_dispatch(first, last, value, iterator_category(first));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  // erange_dispatch 的 forward iterator 版本
  template <class ForwardIter, class T, class Compared>
  zfwstl::pair<ForwardIter, ForwardIter>
  erange_dispatch(ForwardIter first, ForwardIter last,
                  const T &value, forward_iterator_tag, Compared comp)
  {
    auto len = zfwstl::distance(first, last);
    auto half = len;
    ForwardIter middle, left, right;
    while (len > 0)
    {
      half = len >> 1;
      middle = first;
      zfwstl::advance(middle, half);
      if (comp(*middle, value))
      {
        first = middle;
        ++first;
        len = len - half - 1;
      }
      else if (comp(value, *middle))
      {
        len = half;
      }
      else
      {
        left = zfwstl::lower_bound(first, last, value, comp);
        zfwstl::advance(first, len);
        right = zfwstl::upper_bound(++middle, first, value, comp);
        return zfwstl::pair<ForwardIter, ForwardIter>(left, right);
      }
    }
    return zfwstl::pair<ForwardIter, ForwardIter>(last, last);
  }

  // erange_dispatch 的 random access iterator 版本
  template <class RandomIter, class T, class Compared>
  zfwstl::pair<RandomIter, RandomIter>
  erange_dispatch(RandomIter first, RandomIter last,
                  const T &value, random_access_iterator_tag, Compared comp)
  {
    auto len = last - first;
    auto half = len;
    RandomIter middle, left, right;
    while (len > 0)
    {
      half = len >> 1;
      middle = first + half;
      if (comp(*middle, value))
      {
        first = middle + 1;
        len = len - half - 1;
      }
      else if (comp(value, *middle))
      {
        len = half;
      }
      else
      {
        left = zfwstl::lower_bound(first, middle, value, comp);
        right = zfwstl::upper_bound(++middle, first + len, value, comp);
        return zfwstl::pair<RandomIter, RandomIter>(left, right);
      }
    }
    return zfwstl::pair<RandomIter, RandomIter>(last, last);
  }

  template <class ForwardIter, class T, class Compared>
  zfwstl::pair<ForwardIter, ForwardIter>
  equal_range(ForwardIter first, ForwardIter last, const T &value, Compared comp)
  {
    return zfwstl::erange_dispatch(first, last, value, iterator_category(first), comp);
  }

  // ===========================find===========================
  // 顺序查找，在[first, last)区间内找到等于 value 的元素，返回指向该元素的迭代器
  template <class InputIter, class T>
  InputIter
  find(InputIter first, InputIter last, const T &value)
  {
    while (first != last && *first != value)
      ++first;
    return first;
  }

  // ===========================find_if===========================
  // 在[first, last)区间内找到第一个令一元操作 unary_pred 为 true 的元素并返回指向该元素的迭代器
  template <class InputIter, class UnaryPredicate>
  InputIter
  find_if(InputIter first, InputIter last, UnaryPredicate unary_pred)
  {
    while (first != last && !unary_pred(*first))
      ++first;
    return first;
  }
  // ===========================find_if_not===========================
  // 在[first, last)区间内找到第一个令一元操作 unary_pred 为 false 的元素并返回指向该元素的迭代器
  template <class InputIter, class UnaryPredicate>
  InputIter
  find_if_not(InputIter first, InputIter last, UnaryPredicate unary_pred)
  {
    while (first != last && unary_pred(*first))
      ++first;
    return first;
  }

  // ===========================find_end===========================
  // 在[first1, last1)区间中查找[first2, last2)最后一次出现的地方，若不存在返回 last1
  // find_end_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter1, class ForwardIter2>
  ForwardIter1
  find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2,
                    forward_iterator_tag, forward_iterator_tag)
  {
    if (first2 == last2)
    {
      return last1;
    }
    else
    {
      auto result = last1;
      while (true)
      {
        // 利用 search 查找某个子序列的首次出现点，找不到则返回 last1
        auto new_result = zfwstl::search(first1, last1, first2, last2);
        if (new_result == last1)
        {
          return result;
        }
        else
        {
          result = new_result;
          first1 = new_result;
          ++first1;
        }
      }
    }
  }

  // find_end_dispatch 的 bidirectional_iterator_tag 版本(可逆向查找)
  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter1
  find_end_dispatch(BidirectionalIter1 first1, BidirectionalIter1 last1,
                    BidirectionalIter2 first2, BidirectionalIter2 last2,
                    bidirectional_iterator_tag, bidirectional_iterator_tag)
  {
    typedef reverse_iterator<BidirectionalIter1> reviter1;
    typedef reverse_iterator<BidirectionalIter2> reviter2;
    reviter1 rlast1(first1);
    reviter2 rlast2(first2);
    // 查找是，将序列1和2统统逆转方向
    reviter1 rresult = zfwstl::search(reviter1(last1), rlast1, reviter2(last2), rlast2);
    if (rresult == rlast1)
    {
      return last1;
    }
    else
    {
      // NOTE：为啥最后要将逆向迭代器转回正向迭代器，而不直接移动逆向迭代器？
      // 因为这二者之间有奇妙的“实体关系”和“逻辑关系”
      auto result = rresult.base();                              // 转回正常(非逆向)迭代器
      zfwstl::advance(result, -zfwstl::distance(first2, last2)); // 调整回子序列的起头处
      return result;
    }
  }

  template <class ForwardIter1, class ForwardIter2>
  ForwardIter1
  find_end(ForwardIter1 first1, ForwardIter1 last1,
           ForwardIter2 first2, ForwardIter2 last2)
  {
    typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
    typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
    return zfwstl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2());
  }

  // 重载版本使用函数对象 comp 代替比较操作
  // find_end_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter1, class ForwardIter2, class Compared>
  ForwardIter1
  find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2,
                    forward_iterator_tag, forward_iterator_tag, Compared comp)
  {
    if (first2 == last2)
    {
      return last1;
    }
    else
    {
      auto result = last1;
      while (true)
      {
        // 利用 search 查找某个子序列的首次出现点，找不到则返回 last1
        auto new_result = zfwstl::search(first1, last1, first2, last2, comp);
        if (new_result == last1)
        {
          return result;
        }
        else
        {
          result = new_result;
          first1 = new_result;
          ++first1;
        }
      }
    }
  }

  // find_end_dispatch 的 bidirectional_iterator_tag 版本(可逆向查找)
  template <class BidirectionalIter1, class BidirectionalIter2, class Compared>
  BidirectionalIter1
  find_end_dispatch(BidirectionalIter1 first1, BidirectionalIter1 last1,
                    BidirectionalIter2 first2, BidirectionalIter2 last2,
                    bidirectional_iterator_tag, bidirectional_iterator_tag, Compared comp)
  {
    typedef reverse_iterator<BidirectionalIter1> reviter1;
    typedef reverse_iterator<BidirectionalIter2> reviter2;
    reviter1 rlast1(first1);
    reviter2 rlast2(first2);
    reviter1 rresult = zfwstl::search(reviter1(last1), rlast1, reviter2(last2), rlast2, comp);
    if (rresult == rlast1)
    {
      return last1;
    }
    else
    {
      auto result = rresult.base();
      zfwstl::advance(result, -zfwstl::distance(first2, last2));
      return result;
    }
  }

  template <class ForwardIter1, class ForwardIter2, class Compared>
  ForwardIter1
  find_end(ForwardIter1 first1, ForwardIter1 last1,
           ForwardIter2 first2, ForwardIter2 last2, Compared comp)
  {
    typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
    typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
    // TAG: 型别名称之后直接加上一对小括号，便会产生一个临时对象, 如Category1()
    return zfwstl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2(), comp);
  }

  // ===========================find_first_of===========================
  // 在[first1, last1)中查找[first2, last2)中的某些元素，返回指向第一次出现的元素的迭代器
  template <class InputIter, class ForwardIter>
  InputIter
  find_first_of(InputIter first1, InputIter last1,
                ForwardIter first2, ForwardIter last2)
  {
    for (; first1 != last1; ++first1)
    {
      for (auto iter = first2; iter != last2; ++iter)
      {
        if (*first1 == *iter)
          return first1;
      }
    }
    return last1;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter, class ForwardIter, class Compared>
  InputIter
  find_first_of(InputIter first1, InputIter last1,
                ForwardIter first2, ForwardIter last2, Compared comp)
  {
    for (; first1 != last1; ++first1)
    {
      for (auto iter = first2; iter != last2; ++iter)
      {
        if (comp(*first1, *iter))
          return first1;
      }
    }
    return last1;
  }

  // ===========================for_each===========================
  // 使用一个函数对象(仿函数) f 对[first, last)区间内的每个元素执行一个 operator() 操作，但不能改变元素内容
  /**
   *  f() 可返回一个值，但该值会被忽略
   *  f不可以改变元素内容，因为first, last都是 InputIter迭代器
   * 如果想一一修改元素内容，应配合transform()使用，且f返回的值会被忽略
   */
  template <class InputIter, class Function>
  Function for_each(InputIter first, InputIter last, Function f)
  {
    for (; first != last; ++first)
    {
      f(*first); // 调用仿函数f的function call操作符，返回值被忽略
    }
    return f;
  }

  // ===========================generate===========================
  // 将函数对象 gen 的运算结果对[first, last)内的每个元素赋值
  // 用的是迭代器所知元素的赋值(assignment)操作符
  template <class ForwardIter, class Generator>
  void generate(ForwardIter first, ForwardIter last, Generator gen)
  {
    for (; first != last; ++first)
      *first = gen();
  }

  // ===========================generate_n===========================
  // 用函数对象 gen 连续对 n 个元素赋值
  template <class ForwardIter, class Size, class Generator>
  void generate_n(ForwardIter first, Size n, Generator gen)
  {
    for (; n > 0; --n, ++first)
      *first = gen();
  }

  // ===========================includes===========================
  // 判断序列一S1 是否包含序列二S2
  template <class InputIter1, class InputIter2>
  bool includes(InputIter1 first1, InputIter1 last1,
                InputIter2 first2, InputIter2 last2)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (*first2 < *first1)
      {
        return false;
      }
      else if (*first1 < *first2)
      {
        ++first1;
      }
      else
      {
        ++first1, ++first2;
      }
    }
    return first2 == last2;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  /**
   * 这个comp可能会造成整个includes算法语法错误。所以要注意！
   * comp的型别是Compare，既不是BinaryPredicate, 也不是BinaryOperation, 所以并非随便一个二元运算就可以拿来作为comp的参数的
   */
  template <class InputIter1, class InputIter2, class Compared>
  bool includes(InputIter1 first1, InputIter1 last1,
                InputIter2 first2, InputIter2 last2, Compared comp)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (comp(*first2, *first1))
      {
        return false;
      }
      else if (comp(*first1, *first2))
      {
        ++first1;
      }
      else
      {
        ++first1, ++first2;
      }
    }
    return first2 == last2;
  }

  // ===========================inplace_merge===========================
  // 把连接在一起的两个有序序列结合成单一序列并保持有序
  // 稳定操作
  // 没有缓冲区的情况下合并, 它的功效和STL的rotate并没有什么不同，只是它针对缓冲区的存在，做了优化
  template <class BidirectionalIter, class Distance>
  void merge_without_buffer(BidirectionalIter first, BidirectionalIter middle,
                            BidirectionalIter last, Distance len1, Distance len2)
  {
    if (len1 == 0 || len2 == 0)
      return;
    if (len1 + len2 == 2)
    {
      if (*middle < *first)
        zfwstl::iter_swap(first, middle);
      return;
    }
    auto first_cut = first;
    auto second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2)
    { // 序列一较长，找到序列一的中点
      len11 = len1 >> 1;
      zfwstl::advance(first_cut, len11);
      second_cut = zfwstl::lower_bound(middle, last, *first_cut);
      len22 = zfwstl::distance(middle, second_cut);
    }
    else
    { // 序列二较长，找到序列二的中点
      len22 = len2 >> 1;
      zfwstl::advance(second_cut, len22);
      first_cut = zfwstl::upper_bound(first, middle, *second_cut);
      len11 = zfwstl::distance(first, first_cut);
    }
    auto new_middle = zfwstl::rotate(first_cut, middle, second_cut);
    zfwstl::merge_without_buffer(first, first_cut, new_middle, len11, len22);
    zfwstl::merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22);
  }

  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter1
  merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
                 BidirectionalIter2 first2, BidirectionalIter2 last2,
                 BidirectionalIter1 result)
  {
    if (first1 == last1)
      return zfwstl::copy_backward(first2, last2, result);
    if (first2 == last2)
      return zfwstl::copy_backward(first1, last1, result);
    --last1;
    --last2;
    while (true)
    {
      if (*last2 < *last1)
      {
        *--result = *last1;
        if (first1 == last1)
          return zfwstl::copy_backward(first2, ++last2, result);
        --last1;
      }
      else
      {
        *--result = *last2;
        if (first2 == last2)
          return zfwstl::copy_backward(first1, ++last1, result);
        --last2;
      }
    }
  }

  template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
  BidirectionalIter1
  rotate_adaptive(BidirectionalIter1 first, BidirectionalIter1 middle,
                  BidirectionalIter1 last, Distance len1, Distance len2,
                  BidirectionalIter2 buffer, Distance buffer_size)
  {
    BidirectionalIter2 buffer_end;
    if (len1 > len2 && len2 <= buffer_size)
    {
      buffer_end = zfwstl::copy(middle, last, buffer);
      zfwstl::copy_backward(first, middle, last);
      return zfwstl::copy(buffer, buffer_end, first);
    }
    else if (len1 <= buffer_size)
    {
      buffer_end = zfwstl::copy(first, middle, buffer);
      zfwstl::copy(middle, last, first);
      return zfwstl::copy_backward(buffer, buffer_end, last);
    }
    else
    { // 缓冲区仍然不足，改用rotate
      return zfwstl::rotate(first, middle, last);
    }
  }

  // 有缓冲区的情况下合并
  template <class BidirectionalIter, class Distance, class Pointer>
  void merge_adaptive(BidirectionalIter first, BidirectionalIter middle,
                      BidirectionalIter last, Distance len1, Distance len2,
                      Pointer buffer, Distance buffer_size)
  {
    // 区间长度足够放进缓冲区
    if (len1 <= len2 && len1 <= buffer_size)
    { // case1
      Pointer buffer_end = zfwstl::copy(first, middle, buffer);
      zfwstl::merge(buffer, buffer_end, middle, last, first);
    }
    else if (len2 <= buffer_size)
    { // case2
      Pointer buffer_end = zfwstl::copy(middle, last, buffer);
      zfwstl::merge_backward(first, middle, buffer, buffer_end, last);
    }
    else
    { // case3
      // 区间长度太长，分割递归处理
      auto first_cut = first;
      auto second_cut = middle;
      Distance len11 = 0;
      Distance len22 = 0;
      if (len1 > len2)
      {
        len11 = len1 >> 1;
        zfwstl::advance(first_cut, len11);
        second_cut = zfwstl::lower_bound(middle, last, *first_cut);
        len22 = zfwstl::distance(middle, second_cut);
      }
      else
      {
        len22 = len2 >> 1;
        zfwstl::advance(second_cut, len22);
        first_cut = zfwstl::upper_bound(first, middle, *second_cut);
        len11 = zfwstl::distance(first, first_cut);
      }
      auto new_middle = zfwstl::rotate_adaptive(first_cut, middle, second_cut,
                                                len1 - len11, len22, buffer, buffer_size);
      // 针对左段，递归调用
      zfwstl::merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
      // 针对右段，递归调用
      zfwstl::merge_adaptive(new_middle, second_cut, last, len1 - len11,
                             len2 - len22, buffer, buffer_size);
    }
  }

  template <class BidirectionalIter, class T>
  void
  inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle,
                    BidirectionalIter last, T *)
  {
    auto len1 = zfwstl::distance(first, middle);
    auto len2 = zfwstl::distance(middle, last);
    // ！！本算法会使用额外内存空间(暂时缓冲区)-->有它效率会高些
    auto buf = std::get_temporary_buffer<T>(last - first);
    if (!buf.begin())
    { // 内存分配失败
      zfwstl::merge_without_buffer(first, middle, last, len1, len2);
    }
    else
    { // 在有暂时缓冲区的情况进行
      zfwstl::merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size());
    }
  }

  template <class BidirectionalIter>
  void
  inplace_merge(BidirectionalIter first, BidirectionalIter middle,
                BidirectionalIter last)
  {
    if (first == middle || middle == last)
      return;
    zfwstl::inplace_merge_aux(first, middle, last, value_type(first));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  // 没有缓冲区的情况下合并
  template <class BidirectionalIter, class Distance, class Compared>
  void merge_without_buffer(BidirectionalIter first, BidirectionalIter middle,
                            BidirectionalIter last, Distance len1, Distance len2,
                            Compared comp)
  {
    if (len1 == 0 || len2 == 0)
      return;
    if (len1 + len2 == 2)
    {
      if (comp(*middle, *first))
        zfwstl::iter_swap(first, middle);
      return;
    }
    auto first_cut = first;
    auto second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2)
    {
      len11 = len1 >> 1;
      zfwstl::advance(first_cut, len11);
      second_cut = zfwstl::lower_bound(middle, last, *first_cut, comp);
      len22 = zfwstl::distance(middle, second_cut);
    }
    else
    {
      len22 = len2 >> 1;
      zfwstl::advance(second_cut, len22);
      first_cut = zfwstl::upper_bound(first, middle, *second_cut, comp);
      len11 = zfwstl::distance(first, first_cut);
    }
    auto new_middle = zfwstl::rotate(first_cut, middle, second_cut);
    zfwstl::merge_without_buffer(first, first_cut, new_middle, len11, len22, comp);
    zfwstl::merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22, comp);
  }

  template <class BidirectionalIter1, class BidirectionalIter2, class Compared>
  BidirectionalIter1
  merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
                 BidirectionalIter2 first2, BidirectionalIter2 last2,
                 BidirectionalIter1 result, Compared comp)
  {
    if (first1 == last1)
      return zfwstl::copy_backward(first2, last2, result);
    if (first2 == last2)
      return zfwstl::copy_backward(first1, last1, result);
    --last1;
    --last2;
    while (true)
    {
      if (comp(*last2, *last1))
      {
        *--result = *last1;
        if (first1 == last1)
          return zfwstl::copy_backward(first2, ++last2, result);
        --last1;
      }
      else
      {
        *--result = *last2;
        if (first2 == last2)
          return zfwstl::copy_backward(first1, ++last1, result);
        --last2;
      }
    }
  }

  // 有缓冲区的情况下合并
  template <class BidirectionalIter, class Distance, class Pointer, class Compared>
  void merge_adaptive(BidirectionalIter first, BidirectionalIter middle,
                      BidirectionalIter last, Distance len1, Distance len2,
                      Pointer buffer, Distance buffer_size, Compared comp)
  {
    // 区间长度足够放进缓冲区
    if (len1 <= len2 && len1 <= buffer_size)
    {
      Pointer buffer_end = zfwstl::copy(first, middle, buffer);
      zfwstl::merge(buffer, buffer_end, middle, last, first, comp);
    }
    else if (len2 <= buffer_size)
    {
      Pointer buffer_end = zfwstl::copy(middle, last, buffer);
      zfwstl::merge_backward(first, middle, buffer, buffer_end, last, comp);
    }
    else
    { // 区间长度太长，分割递归处理
      auto first_cut = first;
      auto second_cut = middle;
      Distance len11 = 0;
      Distance len22 = 0;
      if (len1 > len2)
      {
        len11 = len1 >> 1;
        zfwstl::advance(first_cut, len11);
        second_cut = zfwstl::lower_bound(middle, last, *first_cut, comp);
        len22 = zfwstl::distance(middle, second_cut);
      }
      else
      {
        len22 = len2 >> 1;
        zfwstl::advance(second_cut, len22);
        first_cut = zfwstl::upper_bound(first, middle, *second_cut, comp);
        len11 = zfwstl::distance(first, first_cut);
      }
      auto new_middle = zfwstl::rotate_adaptive(first_cut, middle, second_cut, len1 - len11,
                                                len22, buffer, buffer_size);
      zfwstl::merge_adaptive(first, first_cut, new_middle, len11,
                             len22, buffer, buffer_size, comp);
      zfwstl::merge_adaptive(new_middle, second_cut, last, len1 - len11,
                             len2 - len22, buffer, buffer_size, comp);
    }
  }

  template <class BidirectionalIter, class T, class Compared>
  void
  inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle,
                    BidirectionalIter last, T *, Compared comp)
  {
    auto len1 = zfwstl::distance(first, middle);
    auto len2 = zfwstl::distance(middle, last);
    auto buf = std::get_temporary_buffer<T>(last - first);
    if (!buf.begin())
    {
      zfwstl::merge_without_buffer(first, middle, last, len1, len2, comp);
    }
    else
    {
      zfwstl::merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size(), comp);
    }
  }

  template <class BidirectionalIter, class Compared>
  inline void
  inplace_merge(BidirectionalIter first, BidirectionalIter middle,
                BidirectionalIter last, Compared comp)
  {
    if (first == middle || middle == last)
      return;
    zfwstl::inplace_merge_aux(first, middle, last, value_type(first), comp);
  }

  // ===========================is_heap===========================
  // 检查[first, last)内的元素是否为一个堆，如果是，则返回 true
  template <class RandomIter>
  bool is_heap(RandomIter first, RandomIter last)
  {
    auto n = zfwstl::distance(first, last);
    auto parent = 0;
    for (auto child = 1; child < n; ++child)
    {
      if (first[parent] < first[child])
        return false;
      if ((child & 1) == 0) // 如果 child 是偶数，那么它的最低位是 0; 反之
        ++parent;
    }
    return true;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class RandomIter, class Compared>
  bool is_heap(RandomIter first, RandomIter last, Compared comp)
  {
    auto n = zfwstl::distance(first, last);
    auto parent = 0;
    for (auto child = 1; child < n; ++child)
    {
      if (comp(first[parent], first[child]))
        return false;
      if ((child & 1) == 0)
        ++parent;
    }
    return true;
  }

  // ===========================is_sorted===========================
  // 检查[first, last)内的元素是否升序，如果是升序，则返回 true
  template <class ForwardIter>
  bool is_sorted(ForwardIter first, ForwardIter last)
  {
    if (first == last)
      return true;
    auto next = first;
    ++next;
    for (; next != last; first = next, ++next)
    {
      if (*next < *first)
        return false;
    }
    return true;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class ForwardIter, class Compared>
  bool is_sorted(ForwardIter first, ForwardIter last, Compared comp)
  {
    if (first == last)
      return true;
    auto next = first;
    ++next;
    for (; next != last; first = next, ++next)
    {
      if (comp(*next, *first))
        return false;
    }
    return true;
  }

  // ===========================lower_bound===========================
  /**
   * 使用lower_bound()，序列需先排序
   * 在[first, last)中查找第一个不小于 value 的元素，并返回指向它的迭代器，若没有则返回 last
   */
  // lbound_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter, class T>
  ForwardIter
  lbound_dispatch(ForwardIter first, ForwardIter last,
                  const T &value, forward_iterator_tag)
  {
    auto len = zfwstl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first;
      zfwstl::advance(middle, half);
      if (*middle < value)
      {
        first = middle;
        ++first;
        len = len - half - 1;
      }
      else
      {
        len = half;
      }
    }
    return first;
  }

  // lbound_dispatch 的 random_access_iterator_tag 版本
  template <class RandomIter, class T>
  RandomIter
  lbound_dispatch(RandomIter first, RandomIter last,
                  const T &value, random_access_iterator_tag)
  {
    auto len = last - first;
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first + half;
      if (*middle < value)
      {
        first = middle + 1;
        len = len - half - 1;
      }
      else
      {
        len = half;
      }
    }
    return first;
  }

  template <class ForwardIter, class T>
  ForwardIter
  lower_bound(ForwardIter first, ForwardIter last, const T &value)
  {
    return zfwstl::lbound_dispatch(first, last, value, iterator_category(first));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  // lbound_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter, class T, class Compared>
  ForwardIter
  lbound_dispatch(ForwardIter first, ForwardIter last,
                  const T &value, forward_iterator_tag, Compared comp)
  {
    auto len = zfwstl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first;
      zfwstl::advance(middle, half);
      if (comp(*middle, value))
      {
        first = middle;
        ++first;
        len = len - half - 1;
      }
      else
      {
        len = half;
      }
    }
    return first;
  }

  // lbound_dispatch 的 random_access_iterator_tag 版本
  template <class RandomIter, class T, class Compared>
  RandomIter
  lbound_dispatch(RandomIter first, RandomIter last,
                  const T &value, random_access_iterator_tag, Compared comp)
  {
    auto len = last - first;
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first + half;
      if (comp(*middle, value))
      {
        first = middle + 1;
        len = len - half - 1;
      }
      else
      {
        len = half;
      }
    }
    return first;
  }

  template <class ForwardIter, class T, class Compared>
  ForwardIter
  lower_bound(ForwardIter first, ForwardIter last, const T &value, Compared comp)
  {
    return zfwstl::lbound_dispatch(first, last, value, iterator_category(first), comp);
  }

  // ===========================max_element===========================
  // 返回一个迭代器，指向序列中最大的元素
  template <class ForwardIter>
  ForwardIter max_element(ForwardIter first, ForwardIter last)
  {
    if (first == last)
      return first;
    auto result = first;
    while (++first != last)
    {
      if (*result < *first)
        result = first;
    }
    return result;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class ForwardIter, class Compared>
  ForwardIter max_element(ForwardIter first, ForwardIter last, Compared comp)
  {
    if (first == last)
      return first;
    auto result = first;
    while (++first != last)
    {
      if (comp(*result, *first))
        result = first;
    }
    return result;
  }

  // ===========================merge===========================
  // 将两个经过排序的集合 S1 和 S2 合并起来置于另一段空间，返回一个迭代器指向最后一个元素的下一位置
  template <class InputIter1, class InputIter2, class OutputIter>
  OutputIter
  merge(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (*first2 < *first1)
      {
        *result = *first2;
        ++first2;
      }
      else
      {
        *result = *first1;
        ++first1;
      }
      ++result;
    }
    return zfwstl::copy(first2, last2, zfwstl::copy(first1, last1, result));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter1, class InputIter2, class OutputIter, class Compared>
  OutputIter
  merge(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result, Compared comp)
  {
    while (first1 != last1 && first2 != last2)
    {
      if (comp(*first2, *first1))
      {
        *result = *first2;
        ++first2;
      }
      else
      {
        *result = *first1;
        ++first1;
      }
      ++result;
    }
    return zfwstl::copy(first2, last2, zfwstl::copy(first1, last1, result));
  }

  // ===========================merge_sort===========================
  // 归并排序
  template <class BidirectionalIter>
  void merge_sort(BidirectionalIter first, BidirectionalIter last)
  {
    typename zfwstl::iterator_traits<BidirectionalIter>::difference_type n = zfwstl::distance(first, last);
    if (n == 0 || n == 1)
      return;
    else
    {
      auto mid = first + n / 2;
      // 对半分割的操作递归进行，知道每一小段长度=0或1(该小段算是自动完成排序)
      merge_sort(first, mid);
      merge_sort(mid, last);
      inplace_merge(first, mid, last); // 利用inplace_merge重新组合成一个完整的有序序列
    }
  }

  // ===========================min_element===========================
  // 返回一个迭代器，指向序列中最小的元素
  template <class ForwardIter>
  ForwardIter min_element(ForwardIter first, ForwardIter last)
  {
    if (first == last)
      return first;
    auto result = first;
    while (++first != last)
    {
      if (*first < *result)
        result = first;
    }
    return result;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class ForwardIter, class Compared>
  ForwardIter min_element(ForwardIter first, ForwardIter last, Compared comp)
  {
    if (first == last)
      return first;
    auto result = first;
    while (++first != last)
    {
      if (comp(*first, *result))
        result = first;
    }
    return result;
  }

  // ===========================next_permutation===========================
  // 取得[first, last)所标示序列的下一个排列组合，如果没有下一个排序组合，返回 false，否则返回 true
  template <class BidirectionalIter>
  bool next_permutation(BidirectionalIter first, BidirectionalIter last)
  {
    auto i = last;
    if (first == last || first == --i)
      return false;
    for (;;)
    {
      auto ii = i;
      if (*--i < *ii)
      { // 找到第一对小于关系的元素
        auto j = last;
        while (!(*i < *--j))
        {
        }
        zfwstl::iter_swap(i, j);   // 交换 i，j 所指元素
        zfwstl::reverse(ii, last); // 将 ii 之后的所有元素反转
        return true;
      }
      if (i == first)
      {
        zfwstl::reverse(first, last);
        return false;
      }
    }
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class BidirectionalIter, class Compared>
  bool next_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp)
  {
    auto i = last;
    if (first == last || first == --i)
      return false;
    for (;;)
    {
      auto ii = i;
      if (comp(*--i, *ii))
      {
        auto j = last;
        while (!comp(*i, *--j))
        {
        }
        zfwstl::iter_swap(i, j);   // 交换 i，j 所指元素
        zfwstl::reverse(ii, last); // 将 ii 之后的所有元素反转
        return true;
      }
      if (i == first)
      {
        zfwstl::reverse(first, last);
        return false;
      }
    }
  }

  // //===========================is_permutation===========================
  // 判断[first1,last1)是否为[first2, last2)的排列组合
  template <class ForwardIter1, class ForwardIter2, class BinaryPred>
  bool is_permutation_aux(ForwardIter1 first1, ForwardIter1 last1,
                          ForwardIter2 first2, ForwardIter2 last2,
                          BinaryPred pred)
  {
    constexpr bool is_ra_it = zfwstl::is_random_access_iterator<ForwardIter1>::value && zfwstl::is_random_access_iterator<ForwardIter2>::value;
    if (is_ra_it)
    {
      auto len1 = last1 - first1;
      auto len2 = last2 - first2;
      if (len1 != len2)
        return false;
    }

    // 先找出相同的前缀段
    for (; first1 != last1 && first2 != last2; ++first1, (void)++first2)
    {
      if (!pred(*first1, *first2))
        break;
    }
    if (is_ra_it)
    {
      if (first1 == last1)
        return true;
    }
    else
    {
      auto len1 = zfwstl::distance(first1, last1);
      auto len2 = zfwstl::distance(first2, last2);
      if (len1 == 0 && len2 == 0)
        return true;
      if (len1 != len2)
        return false;
    }

    // 判断剩余部分
    for (auto i = first1; i != last1; ++i)
    {
      bool is_repeated = false;
      for (auto j = first1; j != i; ++j)
      {
        if (pred(*j, *i))
        {
          is_repeated = true;
          break;
        }
      }

      if (!is_repeated)
      {
        // 计算 *i 在 [first2, last2) 的数目
        auto c2 = 0;
        for (auto j = first2; j != last2; ++j)
        {
          if (pred(*i, *j))
            ++c2;
        }
        if (c2 == 0)
          return false;

        // 计算 *i 在 [first1, last1) 的数目
        auto c1 = 1;
        auto j = i;
        for (++j; j != last1; ++j)
        {
          if (pred(*i, *j))
            ++c1;
        }
        if (c1 != c2)
          return false;
      }
    }
    return true;
  }

  template <class ForwardIter1, class ForwardIter2, class BinaryPred>
  bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                      ForwardIter2 first2, ForwardIter2 last2,
                      BinaryPred pred)
  {
    return is_permutation_aux(first1, last1, first2, last2, pred);
  }

  template <class ForwardIter1, class ForwardIter2>
  bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                      ForwardIter2 first2, ForwardIter2 last2)
  {
    typedef typename iterator_traits<ForwardIter1>::value_type v1;
    typedef typename iterator_traits<ForwardIter2>::value_type v2;
    static_assert(std::is_same<v1, v2>::value,
                  "the type should be same in mystl::is_permutation");
    return is_permutation_aux(first1, last1, first2, last2,
                              zfwstl::equal_to<v1>());
  }

  //===========================nth_element===========================
  // 对序列重排，使得所有小于第 n 个元素的元素出现在它的前面，大于它的出现在它的后面
  // 由于它比partial_sort的保证更少，因此比partial_sort较快
  // 做法：不断以median-of-3 partitioning(以首尾中三点中值为枢纽之分割法)
  template <class RandomIter>
  void nth_element(RandomIter first, RandomIter nth,
                   RandomIter last)
  {
    if (nth == last)
      return;
    while (last - first > 3)
    {
      auto cut = zfwstl::unchecked_partition(first, last, zfwstl::median(*first, *(first + (last - first) / 2), *(last - 1)));
      if (cut <= nth) // 如果 nth 位于右段
        first = cut;  // 对右段进行分割
      else
        last = cut; // 对左段进行分割
    }
    zfwstl::insertion_sort(first, last);
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class RandomIter, class Compared>
  void nth_element(RandomIter first, RandomIter nth,
                   RandomIter last, Compared comp)
  {
    if (nth == last)
      return;
    while (last - first > 3)
    {
      auto cut = zfwstl::unchecked_partition(first, last, zfwstl::median(*first, *(first + (last - first) / 2), *(last - 1)), comp);
      if (cut <= nth) // 如果 nth 位于右段
        first = cut;  // 对右段进行分割
      else
        last = cut; // 对左段进行分割
    }
    zfwstl::insertion_sort(first, last, comp);
  }

  // ===========================partial_sort===========================
  // 对整个序列做部分排序，保证较小的 N 个元素以递增顺序置于[first, first + N)中
  /**
   * 以 middle作为中间位置划分，左部分排序，右边不做任何操作
   * 选择partial_sort的原因：效率比sort高，因为它的任务是找出middle-first个元素
   */
  // TAG: 部分排序使用的是堆排序
  template <class RandomIter>
  void partial_sort(RandomIter first, RandomIter middle,
                    RandomIter last)
  {
    zfwstl::make_heap(first, middle);
    for (auto i = middle; i < last; ++i)
    {
      if (*i < *first)
      {
        zfwstl::__pop_heap_aux(first, middle, i);
      }
    }
    zfwstl::sort_heap(first, middle);
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class RandomIter, class Compared>
  void partial_sort(RandomIter first, RandomIter middle,
                    RandomIter last, Compared comp)
  {
    zfwstl::make_heap(first, middle, comp);
    for (auto i = middle; i < last; ++i)
    {
      if (comp(*i, *first))
      {
        zfwstl::__pop_heap_aux(first, middle, i, comp);
      }
    }
    zfwstl::sort_heap(first, middle, comp);
  }

  // ===========================partial_sort_copy===========================
  // 行为与 partial_sort 类似，不同的是把排序结果复制到 result 容器中
  template <class InputIter, class RandomIter, class Distance>
  RandomIter
  psort_copy_aux(InputIter first, InputIter last,
                 RandomIter result_first, RandomIter result_last,
                 Distance *)
  {
    if (result_first == result_last)
      return result_last;
    auto result_iter = result_first;
    while (first != last && result_iter != result_last)
    {
      *result_iter = *first;
      ++result_iter;
      ++first;
    }
    zfwstl::make_heap(result_first, result_iter);
    while (first != last)
    {
      if (*first < *result_first)
      {
        zfwstl::adjust_heap(result_first, static_cast<Distance>(0),
                            result_iter - result_first, *first);
      }
      ++first;
    }
    zfwstl::sort_heap(result_first, result_iter);
    return result_iter;
  }

  template <class InputIter, class RandomIter>
  RandomIter
  partial_sort_copy(InputIter first, InputIter last,
                    RandomIter result_first, RandomIter result_last)
  {
    return zfwstl::psort_copy_aux(first, last, result_first, result_last,
                                  distance_type(result_first));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter, class RandomIter, class Distance, class Compared>
  RandomIter
  psort_copy_aux(InputIter first, InputIter last,
                 RandomIter result_first, RandomIter result_last,
                 Distance *, Compared comp)
  {
    if (result_first == result_last)
      return result_last;
    auto result_iter = result_first;
    while (first != last && result_iter != result_last)
    {
      *result_iter = *first;
      ++result_iter;
      ++first;
    }
    zfwstl::make_heap(result_first, result_iter, comp);
    while (first != last)
    {
      if (comp(*first, *result_first))
      {
        zfwstl::adjust_heap(result_first, static_cast<Distance>(0),
                            result_iter - result_first, *first, comp);
      }
      ++first;
    }
    zfwstl::sort_heap(result_first, result_iter, comp);
    return result_iter;
  }

  template <class InputIter, class RandomIter, class Compared>
  RandomIter
  partial_sort_copy(InputIter first, InputIter last,
                    RandomIter result_first, RandomIter result_last,
                    Compared comp)
  {
    return zfwstl::psort_copy_aux(first, last, result_first, result_last,
                                  distance_type(result_first), comp);
  }

  // ===========================partition===========================
  // 对区间内的元素重排，被一元条件运算判定为 true 的元素会放到区间的前段
  // 该函数不保证元素的原始相对位置(不稳定)
  template <class BidirectionalIter, class UnaryPredicate>
  BidirectionalIter
  partition(BidirectionalIter first, BidirectionalIter last,
            UnaryPredicate unary_pred)
  {
    while (true)
    {
      //------------操作首部元素------------------------
      while (first != last && unary_pred(*first)) // 头指针所指元素符合不移动条件
      {
        ++first;
      }
      if (first == last) // 头指针=尾指针 所有操作结束
        break;
      // 暂时的此时，first指向不符合条件需要移动的元素位置
      //------------操作尾部元素------------------------
      --last;
      while (first != last && !unary_pred(*last))
      {
        --last;
      }
      if (first == last)
        break;
      zfwstl::iter_swap(first, last); // 交换元素
      ++first;
    }
    return first;
  }

  // ===========================partition_copy===========================
  // 行为与 partition 类似，不同的是，将被一元操作符判定为 true 的放到 result_true 的输出区间
  // 其余放到 result_false 的输出区间，并返回一个 zfwstl::pair 指向这两个区间的尾部
  template <class InputIter, class OutputIter1, class OutputIter2, class UnaryPredicate>
  zfwstl::pair<OutputIter1, OutputIter2>
  partition_copy(InputIter first, InputIter last,
                 OutputIter1 result_true, OutputIter2 result_false,
                 UnaryPredicate unary_pred)
  {
    for (; first != last; ++first)
    {
      if (unary_pred(*first))
        *result_true++ = *first;
      else
        *result_false++ = *first;
    }
    return zfwstl::pair<OutputIter1, OutputIter2>(result_true, result_false);
  }

  // ===========================prev_permutation===========================
  // 取得[first, last)所标示序列的上一个排列组合，如果没有上一个排序组合，返回 false，否则返回 true
  template <class BidirectionalIter>
  bool prev_permutation(BidirectionalIter first, BidirectionalIter last)
  {
    auto i = last;
    if (first == last || first == --i)
      return false;
    for (;;)
    {
      auto ii = i;
      if (*ii < *--i)
      { // 找到第一对大于关系的元素
        auto j = last;
        while (!(*--j < *i))
        {
        }
        zfwstl::iter_swap(i, j);   // 交换i，j
        zfwstl::reverse(ii, last); // 将 ii 之后的所有元素反转
        return true;
      }
      if (i == first)
      {
        zfwstl::reverse(first, last);
        return false;
      }
    }
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class BidirectionalIter, class Compared>
  bool prev_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp)
  {
    auto i = last;
    if (first == last || first == --i)
      return false;
    for (;;)
    {
      auto ii = i;
      if (comp(*ii, *--i))
      {
        auto j = last;
        while (!comp(*--j, *i))
        {
        }
        zfwstl::iter_swap(i, j);   // 交换i，j
        zfwstl::reverse(ii, last); // 将 ii 之后的所有元素反转
        return true;
      }
      if (i == first)
      {
        zfwstl::reverse(first, last);
        return false;
      }
    }
  }
  // ===========================random_shuffle===========================
  // 将[first, last)内的元素次序随机重排
  // 重载版本使用一个产生随机数的函数对象 rand
  template <class RandomIter>
  void random_shuffle(RandomIter first, RandomIter last)
  {
    if (first == last)
      return;
    srand((unsigned)time(0));
    for (auto i = first + 1; i != last; ++i)
    {
      zfwstl::iter_swap(i, first + (rand() % (i - first + 1)));
    }
  }

  // 重载版本使用一个产生随机数的函数对象 rand
  template <class RandomIter, class RandomNumberGenerator>
  void random_shuffle(RandomIter first, RandomIter last,
                      RandomNumberGenerator &rand)
  {
    if (first == last)
      return;
    auto len = zfwstl::distance(first, last);
    for (auto i = first + 1; i != last; ++i)
    {
      zfwstl::iter_swap(i, first + (rand(i - first + 1) % len));
    }
  }
  // ===========================random_sample、random_sample_n===========================
  // 随机取样
  template <class ForwardIter, class OutputIter, class Distance>
  OutputIter random_sample_n(ForwardIter __first, ForwardIter __last,
                             OutputIter __out, const Distance __n)
  {
    Distance __remaining = 0;
    zfwstl::distance(__first, __last, __remaining);
    Distance __m = zfwstl::min(__n, __remaining);

    while (__m > 0)
    {
      if (__random_number(__remaining) < __m)
      {
        *__out = *__first;
        ++__out;
        --__m;
      }

      --__remaining;
      ++__first;
    }
    return __out;
  }

  template <class ForwardIter, class OutputIter, class Distance,
            class _RandomNumberGenerator>
  OutputIter random_sample_n(ForwardIter __first, ForwardIter __last,
                             OutputIter __out, const Distance __n,
                             _RandomNumberGenerator &__rand)
  {

    // __STL_UNARY_FUNCTION_CHECK(_RandomNumberGenerator, Distance, Distance);
    Distance __remaining = 0;
    zfwstl::distance(__first, __last, __remaining);
    Distance __m = zfwstl::min(__n, __remaining);

    while (__m > 0)
    {
      if (__rand(__remaining) < __m)
      {
        *__out = *__first;
        ++__out;
        --__m;
      }

      --__remaining;
      ++__first;
    }
    return __out;
  }

  template <class _InputIter, class _RandomAccessIter, class Distance>
  _RandomAccessIter __random_sample(_InputIter __first, _InputIter __last,
                                    _RandomAccessIter __out,
                                    const Distance __n)
  {
    Distance __m = 0;
    Distance __t = __n;
    for (; __first != __last && __m < __n; ++__m, ++__first)
      __out[__m] = *__first;

    while (__first != __last)
    {
      ++__t;
      Distance __M = __random_number(__t);
      if (__M < __n)
        __out[__M] = *__first;
      ++__first;
    }

    return __out + __m;
  }

  template <class _InputIter, class _RandomAccessIter,
            class _RandomNumberGenerator, class Distance>
  _RandomAccessIter __random_sample(_InputIter __first, _InputIter __last,
                                    _RandomAccessIter __out,
                                    _RandomNumberGenerator &__rand,
                                    const Distance __n)
  {
    // __STL_UNARY_FUNCTION_CHECK(_RandomNumberGenerator, Distance, Distance);
    Distance __m = 0;
    Distance __t = __n;
    for (; __first != __last && __m < __n; ++__m, ++__first)
      __out[__m] = *__first;

    while (__first != __last)
    {
      ++__t;
      Distance __M = __rand(__t);
      if (__M < __n)
        __out[__M] = *__first;
      ++__first;
    }

    return __out + __m;
  }

  template <class _InputIter, class _RandomAccessIter>
  inline _RandomAccessIter
  random_sample(_InputIter __first, _InputIter __last,
                _RandomAccessIter __out_first, _RandomAccessIter __out_last)
  {
    // __STL_REQUIRES(_InputIter, _InputIterator);
    // __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
    return __random_sample(__first, __last,
                           __out_first, __out_last - __out_first);
  }

  template <class _InputIter, class _RandomAccessIter,
            class _RandomNumberGenerator>
  inline _RandomAccessIter
  random_sample(_InputIter __first, _InputIter __last,
                _RandomAccessIter __out_first, _RandomAccessIter __out_last,
                _RandomNumberGenerator &__rand)
  {
    // __STL_REQUIRES(_InputIter, _InputIterator);
    // __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
    return __random_sample(__first, __last,
                           __out_first, __rand,
                           __out_last - __out_first);
  }

  // ===========================remove===========================
  // TAG：对于remove及其衍生的相关算法，移除(但不删除)所有与指定 value 相等的元素
  // 算法会留有一些残余数据，若要删除，则可将返回的迭代器交给区间所在之容器的erase()成员函数
  // NOTE: 并不从容器中删除这些元素，所以 remove 和 remove_if 不适用于 array
  // NOTE: 对于array来说，更为多的是remove_copy()和remove_copy_if()
  template <class ForwardIter, class T>
  ForwardIter remove(ForwardIter first, ForwardIter last, const T &value)
  {
    first = zfwstl::find(first, last, value); // 利用 find 找出第一个匹配的地方
    auto next = first;
    // 利用remove_copy()允许新旧容器重叠的性质，进行移除
    return first == last ? first : zfwstl::remove_copy(++next, last, first, value);
  }

  // ===========================remove_copy===========================
  // 移除区间内与指定 value 相等的元素，并将结果复制到以 result 标示起始位置的容器上
  // 新容器可以和原容器重叠，但如果对新容器实际给值时，超越了旧容器的大小，会产生无法预期的结果
  template <class InputIter, class OutputIter, class T>
  OutputIter
  remove_copy(InputIter first, InputIter last, OutputIter result, const T &value)
  {
    for (; first != last; ++first)
      if (*first != value)
        *result++ = *first; // 赋值给新容器
    return result;          // 返回OutputIter迭代器指向的被复制的最后元素的下一位置！！关键！！
  }

  // ===========================remove_copy_if===========================
  // 移除区间内所有令一元操作 unary_pred 为 true 的元素，并将结果复制到以 result 为起始位置的容器上
  template <class InputIter, class OutputIter, class UnaryPredicate>
  OutputIter
  remove_copy_if(InputIter first, InputIter last,
                 OutputIter result, UnaryPredicate unary_pred)
  {
    for (; first != last; ++first)
    {
      if (!unary_pred(*first))
      {
        *result = *first;
        ++result;
      }
    }
    return result;
  }

  // ===========================remove_if===========================
  // 移除区间内所有令一元操作 unary_pred 为 true 的元素
  template <class ForwardIter, class UnaryPredicate>
  ForwardIter
  remove_if(ForwardIter first, ForwardIter last, UnaryPredicate unary_pred)
  {
    first = zfwstl::find_if(first, last, unary_pred); // 利用 find_if 找出第一个匹配的地方
    auto next = first;
    // 新容器可以和原容器重叠
    return first == last ? first : zfwstl::remove_copy_if(++next, last, first, unary_pred);
  }

  // ===========================replace===========================
  // 将区间内所有的 old_value 都以 new_value 替代
  template <class ForwardIter, class T>
  void replace(ForwardIter first, ForwardIter last,
               const T &old_value, const T &new_value)
  {
    for (; first != last; ++first)
    {
      if (*first == old_value)
        *first = new_value;
    }
  }

  // ===========================replace_copy===========================
  // 行为与 replace 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
  template <class InputIter, class OutputIter, class T>
  OutputIter
  replace_copy(InputIter first, InputIter last,
               OutputIter result, const T &old_value, const T &new_value)
  {
    for (; first != last; ++first, ++result)
    {
      *result = *first == old_value ? new_value : *first;
    }
    return result;
  }

  // ===========================replace_if===========================
  // 将区间内所有令一元操作 unary_pred 为 true 的元素都用 new_value 替代
  template <class ForwardIter, class UnaryPredicate, class T>
  void replace_if(ForwardIter first, ForwardIter last,
                  UnaryPredicate unary_pred, const T &new_value)
  {
    for (; first != last; ++first)
    {
      if (unary_pred(*first))
        *first = new_value;
    }
  }

  // ===========================replace_copy_if===========================
  // 行为与 replace_if 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
  template <class InputIter, class OutputIter, class UnaryPredicate, class T>
  OutputIter
  replace_copy_if(InputIter first, InputIter last,
                  OutputIter result, UnaryPredicate unary_pred, const T &new_value)
  {
    for (; first != last; ++first, ++result)
    {
      *result = unary_pred(*first) ? new_value : *first;
    }
    return result;
  }

  // ===========================reverse===========================
  // 将[first, last)区间内的元素反转
  // reverse_dispatch 的 bidirectional_iterator_tag 版本
  template <class BidirectionalIter>
  void reverse_dispatch(BidirectionalIter first, BidirectionalIter last,
                        bidirectional_iterator_tag)
  {
    while (true)
    {
      if (first == last || first == --last)
        return;
      zfwstl::iter_swap(first++, last);
    }
  }

  // reverse_dispatch 的 random_access_iterator_tag 版本
  template <class RandomIter>
  void reverse_dispatch(RandomIter first, RandomIter last,
                        random_access_iterator_tag)
  {
    while (first < last)
      zfwstl::iter_swap(first++, --last);
  }

  template <class BidirectionalIter>
  void reverse(BidirectionalIter first, BidirectionalIter last)
  {
    zfwstl::reverse_dispatch(first, last, iterator_category(first));
  }

  // ===========================reverse_copy===========================
  // 行为与 reverse 类似，不同的是将结果复制到 result 所指容器中
  template <class BidirectionalIter, class OutputIter>
  OutputIter
  reverse_copy(BidirectionalIter first, BidirectionalIter last,
               OutputIter result)
  {
    while (first != last)
    {
      --last;
      *result = *last;
      ++result;
    }
    return result;
  }

  // ===========================rotate===========================
  // 将[first, middle)内的元素和 [middle, last)内的元素互换，可以交换两个长度不同的区间
  // NOTE:区别swap_ranges(), 只能交换两个长度相同的区间
  // 返回交换后 middle 的位置
  // rotate_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter>
  void rotate_dispatch(ForwardIter first, ForwardIter middle,
                       ForwardIter last, forward_iterator_tag)
  {
    for (ForwardIter i = middle;;)
    {
      zfwstl::iter_swap(first, i);
      ++first;
      ++i;
      // 以下判断前端[first, middle)先结束还是后端[middle, last)先结束
      if (first == middle)
      { // 前段结束
        if (i == last)
          return;   // 如果后端也同时结束，则整个结束
        middle = i; // 否则，对新的前后端再做交换
      }
      else if (i == last) // 后段结束
        i = middle;       // 对新的前后端再做交换
    }
  }

  // rotate_dispatch 的 bidirectional_iterator_tag 版本
  template <class BidirectionalIter>
  void rotate_dispatch(BidirectionalIter first, BidirectionalIter middle,
                       BidirectionalIter last, bidirectional_iterator_tag)
  { // 这个妙 ~
    zfwstl::reverse(first, middle);
    zfwstl::reverse(middle, last);
    zfwstl::reverse(first, last);
  }

  // NOTE：求最大公因子，利用辗转相除法
  template <class EuclideanRingElement>
  EuclideanRingElement rgcd(EuclideanRingElement m, EuclideanRingElement n)
  {
    while (n != 0)
    {
      auto t = m % n;
      m = n;
      n = t;
    }
    return m;
  }
  template <class RandomIter, class Distance, class T>
  void __rotate_cycle(RandomIter first, RandomIter last,
                      RandomIter initial, Distance shift, T *)
  {
    T value = *initial;
    RandomIter ptr1 = initial;
    RandomIter ptr2 = ptr1 + shift;
    while (ptr2 != initial)
    {
      *ptr1 = *ptr2;
      ptr1 = ptr2;
      if (last - ptr2 > shift)
        ptr2 += shift;
      else
        ptr2 = first + (shift - (last - ptr2));
    }
    *ptr1 = value;
  }
  // rotate_dispatch 的 random_access_iterator_tag 版本
  template <class RandomIter>
  void rotate_dispatch(RandomIter first, RandomIter middle,
                       RandomIter last, random_access_iterator_tag)
  {
    // 因为是 random access iterator，我们可以确定每个元素的位置
    // 取全长和前段长度的最大公因子
    auto n = rgcd(last - first, middle - first); // n最大公约数将用于减少旋转操作的次数
    while (n--)
      __rotate_cycle(first, last, first + n, middle - first, value_type(first));
  }

  template <class ForwardIter>
  void rotate(ForwardIter first, ForwardIter middle, ForwardIter last)
  {
    if (first == middle || middle == last)
      return;
    zfwstl::rotate_dispatch(first, middle, last, iterator_category(first));
  }

  // ===========================rotate_copy===========================
  // 行为与 rotate 类似，不同的是将结果复制到 result 所指的容器中
  template <class ForwardIter, class OutputIter>
  ForwardIter
  rotate_copy(ForwardIter first, ForwardIter middle,
              ForwardIter last, OutputIter result)
  {
    return zfwstl::copy(first, middle, zfwstl::copy(middle, last, result));
  }

  // ===========================search===========================
  // 在[first1, last1)中查找[first2, last2)的首次出现点
  template <class ForwardIter1, class ForwardIter2>
  ForwardIter1
  search(ForwardIter1 first1, ForwardIter1 last1,
         ForwardIter2 first2, ForwardIter2 last2)
  {
    auto d1 = zfwstl::distance(first1, last1);
    auto d2 = zfwstl::distance(first2, last2);
    if (d1 < d2) // 如果序列2>序列1 不可能成为其子序列
      return last1;
    auto current1 = first1;
    auto current2 = first2;
    while (current2 != last2)
    {
      if (*current1 == *current2)
      {
        ++current1;
        ++current2;
      }
      else
      {
        if (d1 == d2) // 如果两个序列一样长，也不可能成功了
        {
          return last1;
        }
        else
        {
          current1 = ++first1;
          current2 = first2; // 准备在新起点上再找一次
          --d1;              // 已经排除了序列1的一个元素，所以长度-1
        }
      }
    }
    return first1;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class ForwardIter1, class ForwardIter2, class Compared>
  ForwardIter1
  search(ForwardIter1 first1, ForwardIter1 last1,
         ForwardIter2 first2, ForwardIter2 last2, Compared comp)
  {
    auto d1 = zfwstl::distance(first1, last1);
    auto d2 = zfwstl::distance(first2, last2);
    if (d1 < d2)
      return last1;
    auto current1 = first1;
    auto current2 = first2;
    while (current2 != last2)
    {
      if (comp(*current1, *current2))
      {
        ++current1;
        ++current2;
      }
      else
      {
        if (d1 == d2)
        {
          return last1;
        }
        else
        {
          current1 = ++first1;
          current2 = first2;
          --d1;
        }
      }
    }
    return first1;
  }

  // ===========================search_n===========================
  // 在[first, last)中查找连续 n 个 value 所形成的子序列，返回一个迭代器指向该子序列的起始处
  template <class ForwardIter, class Size, class T>
  ForwardIter
  search_n(ForwardIter first, ForwardIter last, Size n, const T &value)
  {
    if (n <= 0)
    {
      return first;
    }
    else
    {
      first = zfwstl::find(first, last, value);
      while (first != last)
      {
        auto m = n - 1;
        auto i = first;
        ++i;
        while (i != last && m != 0 && *i == value)
        {
          ++i;
          --m;
        }
        if (m == 0)
          return first;
        else
          first = zfwstl::find(i, last, value);
      }
      return last;
    }
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class ForwardIter, class Size, class T, class Compared>
  ForwardIter
  search_n(ForwardIter first, ForwardIter last,
           Size n, const T &value, Compared comp)
  {
    if (n <= 0)
    {
      return first;
    }
    else
    {
      while (first != last)
      {
        if (comp(*first, value))
          break;
        ++first;
      }
      while (first != last)
      {
        auto m = n - 1;
        auto i = first;
        ++i;
        while (i != last && m != 0 && comp(*i, value))
        {
          ++i;
          --m;
        }
        if (m == 0)
        {
          return first;
        }
        else
        {
          while (i != last)
          {
            if (comp(*i, value))
              break;
            ++i;
          }
          first = i;
        }
      }
      return last;
    }
  }

  // ===========================sort===========================
  /**
   * STL的排序算法：
   * 数据量大-->快排
   * 一旦分段，数据量小于某个门槛-->插入排序
   * 若递归层次过深-->堆排序
   */
  // 将[first, last)内的元素以递增的方式排序
  constexpr static size_t kSmallSectionSize = 128; // 小型区间的大小，在这个大小内采用插入排序

  // 用于控制分割恶化的情况
  template <class Size>
  Size slg2(Size n)
  { // 找出 2^k <= n 的 k 的最大值
    Size k = 0;
    for (; n > 1; n >>= 1)
      ++k;
    return k;
  }

  // 分割函数 unchecked_partition
  template <class RandomIter, class T>
  RandomIter
  unchecked_partition(RandomIter first, RandomIter last, const T &pivot)
  {
    while (true)
    {
      while (*first < pivot)
        ++first;
      --last;
      while (pivot < *last)
        --last;
      if (!(first < last))
        return first;
      zfwstl::iter_swap(first, last);
      ++first;
    }
  }

  // 内省式排序，先进行 quick sort，当分割行为有恶化倾向时，改用 heap sort
  template <class RandomIter, class Size>
  void intro_sort(RandomIter first, RandomIter last, Size depth_limit)
  {
    while (static_cast<size_t>(last - first) > kSmallSectionSize)
    {
      if (depth_limit == 0)
      {                                          // 到达最大分割深度限制
        zfwstl::partial_sort(first, last, last); // 改用 heap_sort
        return;
      }
      --depth_limit;
      auto mid = zfwstl::median(*(first), *(first + (last - first) / 2), *(last - 1));
      auto cut = zfwstl::unchecked_partition(first, last, mid);
      zfwstl::intro_sort(cut, last, depth_limit);
      last = cut;
    }
  }

  // 插入排序辅助函数 unchecked_linear_insert
  template <class RandomIter, class T>
  void unchecked_linear_insert(RandomIter last, const T &value)
  {
    auto next = last;
    --next;
    while (value < *next)
    {
      *last = *next;
      last = next;
      --next;
    }
    *last = value;
  }

  // 插入排序函数 unchecked_insertion_sort
  template <class RandomIter>
  void unchecked_insertion_sort(RandomIter first, RandomIter last)
  {
    for (auto i = first; i != last; ++i)
    {
      zfwstl::unchecked_linear_insert(i, *i);
    }
  }

  // 插入排序函数 insertion_sort
  template <class RandomIter>
  void insertion_sort(RandomIter first, RandomIter last)
  {
    if (first == last)
      return;
    for (auto i = first + 1; i != last; ++i)
    {
      auto value = *i;
      if (value < *first)
      {
        zfwstl::copy_backward(first, i, i + 1);
        *first = value;
      }
      else
      {
        zfwstl::unchecked_linear_insert(i, value);
      }
    }
  }

  // 最终插入排序函数 final_insertion_sort
  template <class RandomIter>
  void final_insertion_sort(RandomIter first, RandomIter last)
  {
    if (static_cast<size_t>(last - first) > kSmallSectionSize)
    {
      zfwstl::insertion_sort(first, first + kSmallSectionSize);
      zfwstl::unchecked_insertion_sort(first + kSmallSectionSize, last);
    }
    else
    {
      zfwstl::insertion_sort(first, last);
    }
  }

  template <class RandomIter>
  void sort(RandomIter first, RandomIter last)
  {
    if (first != last)
    {
      // 内省式排序，将区间分为一个个小区间，然后对整体进行插入排序
      zfwstl::intro_sort(first, last, slg2(last - first) * 2);
      zfwstl::final_insertion_sort(first, last);
    }
  }

  // 重载版本使用函数对象 comp 代替比较操作
  // 分割函数 unchecked_partition
  template <class RandomIter, class T, class Compared>
  RandomIter
  unchecked_partition(RandomIter first, RandomIter last,
                      const T &pivot, Compared comp)
  {
    while (true)
    {
      while (comp(*first, pivot))
        ++first;
      --last;
      while (comp(pivot, *last))
        --last;
      if (!(first < last))
        return first;
      zfwstl::iter_swap(first, last);
      ++first;
    }
  }

  // 内省式排序，先进行 quick sort，当分割行为有恶化倾向时，改用 heap sort
  template <class RandomIter, class Size, class Compared>
  void intro_sort(RandomIter first, RandomIter last,
                  Size depth_limit, Compared comp)
  {
    while (static_cast<size_t>(last - first) > kSmallSectionSize)
    {
      if (depth_limit == 0)
      {                                                // 到达最大分割深度限制
        zfwstl::partial_sort(first, last, last, comp); // 改用 heap_sort
        return;
      }
      --depth_limit;
      auto mid = zfwstl::median(*(first), *(first + (last - first) / 2), *(last - 1));
      auto cut = zfwstl::unchecked_partition(first, last, mid, comp);
      zfwstl::intro_sort(cut, last, depth_limit, comp);
      last = cut;
    }
  }

  // 插入排序辅助函数 unchecked_linear_insert
  template <class RandomIter, class T, class Compared>
  void unchecked_linear_insert(RandomIter last, const T &value, Compared comp)
  {
    auto next = last;
    --next;
    while (comp(value, *next))
    { // 从尾部开始寻找第一个可插入位置
      *last = *next;
      last = next;
      --next;
    }
    *last = value;
  }

  // 插入排序函数 unchecked_insertion_sort
  template <class RandomIter, class Compared>
  void unchecked_insertion_sort(RandomIter first, RandomIter last,
                                Compared comp)
  {
    for (auto i = first; i != last; ++i)
    {
      zfwstl::unchecked_linear_insert(i, *i, comp);
    }
  }

  // 插入排序函数 insertion_sort
  template <class RandomIter, class Compared>
  void insertion_sort(RandomIter first, RandomIter last, Compared comp)
  {
    if (first == last)
      return;
    for (auto i = first + 1; i != last; ++i)
    {
      auto value = *i;
      if (comp(value, *first))
      {
        zfwstl::copy_backward(first, i, i + 1);
        *first = value;
      }
      else
      {
        zfwstl::unchecked_linear_insert(i, value, comp);
      }
    }
  }

  // 最终插入排序函数 final_insertion_sort
  template <class RandomIter, class Compared>
  void final_insertion_sort(RandomIter first, RandomIter last, Compared comp)
  {
    if (static_cast<size_t>(last - first) > kSmallSectionSize)
    {
      zfwstl::insertion_sort(first, first + kSmallSectionSize, comp);
      zfwstl::unchecked_insertion_sort(first + kSmallSectionSize, last, comp);
    }
    else
    {
      zfwstl::insertion_sort(first, last, comp);
    }
  }

  template <class RandomIter, class Compared>
  void sort(RandomIter first, RandomIter last, Compared comp)
  {
    if (first != last)
    {
      // 内省式排序，将区间分为一个个小区间，然后对整体进行插入排序
      zfwstl::intro_sort(first, last, slg2(last - first) * 2, comp);
      zfwstl::final_insertion_sort(first, last, comp);
    }
  }

  // ===========================stable_partition ===========================
  // 分割并保持元素的相对次序(对应区别 partition)

  // ===========================stable_sort===========================
  // 排序并保持等值元素的相对次序

  // ===========================swap_ranges===========================
  // 将[first1, last1)从 first2 开始，交换相同个数元素
  // 交换的区间长度必须相同，两个序列不能互相重叠，返回一个迭代器指向序列二最后一个被交换元素的下一位置
  template <class ForwardIter1, class ForwardIter2>
  ForwardIter2
  swap_ranges(ForwardIter1 first1, ForwardIter1 last1,
              ForwardIter2 first2)
  {
    for (; first1 != last1; ++first1, ++first2)
    {
      zfwstl::iter_swap(first1, first2);
    }
    return first2;
  }

  // ===========================transform===========================
  // 第一个版本以函数对象 unary_op 作用于[first, last)中的每个元素并将结果保存至 result 中
  template <class InputIter, class OutputIter, class UnaryOperation>
  OutputIter
  transform(InputIter first, InputIter last,
            OutputIter result, UnaryOperation unary_op)
  {
    for (; first != last; ++first, ++result)
    {
      *result = unary_op(*first);
    }
    return result;
  }
  // 第二个版本以函数对象 binary_op 作用于两个序列[first1, last1)、[first2, last2)的相同位置
  template <class InputIter1, class InputIter2, class OutputIter, class BinaryOperation>
  OutputIter
  transform(InputIter1 first1, InputIter1 last1,
            InputIter2 first2, OutputIter result, BinaryOperation binary_op)
  {
    for (; first1 != last1; ++first1, ++first2, ++result)
    {
      *result = binary_op(*first1, *first2);
    }
    return result;
  }

  // ===========================unique_copy===========================
  // 从[first, last)中将元素复制到 result 上，序列必须有序，如果有重复的元素，只会复制一次
  // unique_copy_dispatch 的 forward_iterator_tag 版本
  template <class InputIter, class ForwardIter>
  ForwardIter
  unique_copy_dispatch(InputIter first, InputIter last,
                       ForwardIter result, forward_iterator_tag)
  {
    *result = *first;
    while (++first != last)
    {
      if (*result != *first)
        *++result = *first;
    }
    return ++result;
  }

  // unique_copy_dispatch 的 output_iterator_tag 版本
  // 由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
  template <class InputIter, class OutputIter>
  OutputIter
  unique_copy_dispatch(InputIter first, InputIter last,
                       OutputIter result, output_iterator_tag)
  {
    auto value = *first;
    *result = value;
    while (++first != last)
    {
      if (value != *first)
      {
        value = *first;
        *++result = value;
      }
    }
    return ++result;
  }

  template <class InputIter, class OutputIter>
  OutputIter
  unique_copy(InputIter first, InputIter last, OutputIter result)
  {
    if (first == last)
      return result;
    return zfwstl::unique_copy_dispatch(first, last, result, iterator_category(result));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  // unique_copy_dispatch 的 forward_iterator_tag 版本
  template <class InputIter, class ForwardIter, class Compared>
  ForwardIter
  unique_copy_dispatch(InputIter first, InputIter last,
                       ForwardIter result, forward_iterator_tag, Compared comp)
  {
    *result = *first;
    while (++first != last)
    {
      if (!comp(*result, *first))
        *++result = *first;
    }
    return ++result;
  }

  // unique_copy_dispatch 的 output_iterator_tag 版本
  // 由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
  template <class InputIter, class OutputIter, class Compared>
  OutputIter
  unique_copy_dispatch(InputIter first, InputIter last,
                       OutputIter result, output_iterator_tag, Compared comp)
  {
    auto value = *first;
    *result = value;
    while (++first != last)
    {
      if (!comp(value, *first))
      {
        value = *first;
        *++result = value;
      }
    }
    return ++result;
  }

  template <class InputIter, class OutputIter, class Compared>
  OutputIter
  unique_copy(InputIter first, InputIter last, OutputIter result, Compared comp)
  {
    if (first == last)
      return result;
    return zfwstl::unique_copy_dispatch(first, last, result, iterator_category(result), comp);
  }

  // ===========================unique===========================
  // 移除[first, last)内重复的元素，序列必须有序，和 remove 类似，它也不能真正的删除重复元素
  template <class ForwardIter>
  ForwardIter unique(ForwardIter first, ForwardIter last)
  {
    first = zfwstl::adjacent_find(first, last);
    return zfwstl::unique_copy(first, last, first);
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class ForwardIter, class Compared>
  ForwardIter unique(ForwardIter first, ForwardIter last, Compared comp)
  {
    first = zfwstl::adjacent_find(first, last, comp);
    return zfwstl::unique_copy(first, last, first, comp);
  }

  // ===========================upper_bound===========================
  // 在[first, last)中查找第一个大于value 的元素，并返回指向它的迭代器，若没有则返回 last
  // ubound_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter, class T>
  ForwardIter
  ubound_dispatch(ForwardIter first, ForwardIter last,
                  const T &value, forward_iterator_tag)
  {
    auto len = zfwstl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first;
      zfwstl::advance(middle, half);
      if (value < *middle)
      {
        len = half;
      }
      else
      {
        first = middle;
        ++first;
        len = len - half - 1;
      }
    }
    return first;
  }

  // ubound_dispatch 的 random_access_iterator_tag 版本
  template <class RandomIter, class T>
  RandomIter
  ubound_dispatch(RandomIter first, RandomIter last,
                  const T &value, random_access_iterator_tag)
  {
    auto len = last - first;
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first + half;
      if (value < *middle)
      {
        len = half;
      }
      else
      {
        first = middle + 1;
        len = len - half - 1;
      }
    }
    return first;
  }

  template <class ForwardIter, class T>
  ForwardIter
  upper_bound(ForwardIter first, ForwardIter last, const T &value)
  {
    return zfwstl::ubound_dispatch(first, last, value, iterator_category(first));
  }

  // 重载版本使用函数对象 comp 代替比较操作
  // ubound_dispatch 的 forward_iterator_tag 版本
  template <class ForwardIter, class T, class Compared>
  ForwardIter
  ubound_dispatch(ForwardIter first, ForwardIter last,
                  const T &value, forward_iterator_tag, Compared comp)
  {
    auto len = zfwstl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first;
      zfwstl::advance(middle, half);
      if (comp(value, *middle))
      {
        len = half;
      }
      else
      {
        first = middle;
        ++first;
        len = len - half - 1;
      }
    }
    return first;
  }

  // ubound_dispatch 的 random_access_iterator_tag 版本
  template <class RandomIter, class T, class Compared>
  RandomIter
  ubound_dispatch(RandomIter first, RandomIter last,
                  const T &value, random_access_iterator_tag, Compared comp)
  {
    auto len = last - first;
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
      half = len >> 1;
      middle = first + half;
      if (comp(value, *middle))
      {
        len = half;
      }
      else
      {
        first = middle + 1;
        len = len - half - 1;
      }
    }
    return first;
  }

  template <class ForwardIter, class T, class Compared>
  ForwardIter
  upper_bound(ForwardIter first, ForwardIter last, const T &value, Compared comp)
  {
    return zfwstl::ubound_dispatch(first, last, value, iterator_category(first), comp);
  }

  // ===========================move===========================
  // 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
  // input_iterator_tag 版本
  template <class InputIter, class OutputIter>
  OutputIter
  unchecked_move_cat(InputIter first, InputIter last, OutputIter result,
                     zfwstl::input_iterator_tag)
  {
    for (; first != last; ++first, ++result)
    {
      *result = zfwstl::move(*first);
    }
    return result;
  }

  // ramdom_access_iterator_tag 版本
  template <class RandomIter, class OutputIter>
  OutputIter
  unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result,
                     zfwstl::random_access_iterator_tag)
  {
    for (auto n = last - first; n > 0; --n, ++first, ++result)
    {
      *result = zfwstl::move(*first);
    }
    return result;
  }

  template <class InputIter, class OutputIter>
  OutputIter
  unchecked_move(InputIter first, InputIter last, OutputIter result)
  {
    return unchecked_move_cat(first, last, result, iterator_category(first));
  }

  // 为 trivially_copy_assignable 类型提供特化版本
  template <class Tp, class Up>
  typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
          std::is_trivially_move_assignable<Up>::value,
      Up *>::type
  unchecked_move(Tp *first, Tp *last, Up *result)
  {
    const size_t n = static_cast<size_t>(last - first);
    if (n != 0)
      std::memmove(result, first, n * sizeof(Up));
    return result + n;
  }

  template <class InputIter, class OutputIter>
  OutputIter move(InputIter first, InputIter last, OutputIter result)
  {
    return unchecked_move(first, last, result);
  }

  // ===========================move_backward===========================
  // 将 [first, last)区间内的元素移动到 [result - (last - first), result)内
  // bidirectional_iterator_tag 版本
  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2
  unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                              BidirectionalIter2 result, zfwstl::bidirectional_iterator_tag)
  {
    while (first != last)
      *--result = zfwstl::move(*--last);
    return result;
  }

  // random_access_iterator_tag 版本
  template <class RandomIter1, class RandomIter2>
  RandomIter2
  unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
                              RandomIter2 result, zfwstl::random_access_iterator_tag)
  {
    for (auto n = last - first; n > 0; --n)
      *--result = zfwstl::move(*--last);
    return result;
  }

  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2
  unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                          BidirectionalIter2 result)
  {
    return unchecked_move_backward_cat(first, last, result,
                                       iterator_category(first));
  }

  // 为 trivially_copy_assignable 类型提供特化版本
  template <class Tp, class Up>
  typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
          std::is_trivially_move_assignable<Up>::value,
      Up *>::type
  unchecked_move_backward(Tp *first, Tp *last, Up *result)
  {
    const size_t n = static_cast<size_t>(last - first);
    if (n != 0)
    {
      result -= n;
      std::memmove(result, first, n * sizeof(Up));
    }
    return result;
  }

  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2
  move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
  {
    return unchecked_move_backward(first, last, result);
  }
}

#endif // !ZFWSTL_ALGO_H_
