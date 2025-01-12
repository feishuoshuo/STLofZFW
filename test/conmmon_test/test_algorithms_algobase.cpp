#include <iostream>
#include <string>
#include "../STL/deque.h"
#include "../STL/vector.h"
#include "../src/algorithms/algobase.h" //for copy(), copy_backward()
#include "../src/algorithms/algo.h"     //for for_each()
#include "../src/algorithms/numeric.h"  //for lexicographical_compare()

template <class T>
struct display
{
  void operator()(const T &x) const
  {
    cout << x << ' ';
  }
};

int main()
{
  int ia[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  // int ib = iota(ia, ia + 9, 0); // include numeric

  zfwstl::vector<int> iv1(ia, ia + 5);
  zfwstl::vector<int> iv2(ia, ia + 9);

  // 判断两个序列第一个不匹配点 pair（第一个序列的不匹配点，第二个序列的不匹配点）
  // TODO: 这种写法比较危险，应该先判断迭代器是否不等于容器的end(),然后才可以做输出操作
  std::cout << *(mismatch(iv1.begin(), iv1.end(), iv2.begin()).first);
  std::cout << *(mismatch(iv1.begin(), iv1.end(), iv2.begin()).second);

  // 如果序列在区间内相等，equal返回true
  std::cout << equal(iv1.begin(), iv1.end(), iv2.begin());
  std::cout << equal(iv1.begin(), iv1.end(), &ia[3]);
  std::cout << equal(iv1.begin(), iv1.end(), &ia[3], less<int>());

  zfwstl::fill(iv1.begin(), iv1.end(), 9);
  zfwstl::for_each(iv1.begin(), iv1.end(), display<int>());

  zfwstl::fill_n(iv1.begin(), 3, 7); // 从迭代器开始填3个7
  zfwstl::for_each(iv1.begin(), iv1.end(), display<int>());

  zfwstl::vector<int>::iterator ite1 = iv1.begin();
  zfwstl::vector<int>::iterator ite2 = ite1; // 指向7
  zfwstl::advance(ite2, 3);                  // 指向9，前进

  zfwstl::iter_swap(ite1, ite2);
  cout << *ite1 << ' ' << *ite2 << endl;
  zfwstl::for_each(iv1.begin(), iv1.end(), display<int>());

  cout << zfwstl::max(*ite1, *ite2) << endl;
  cout << zfwstl::min(*ite1, *ite2) << endl;

  // // 错误写法
  // cout << *max(ite1, ite2) << endl;
  // cout << *min(ite1, ite2) << endl;

  zfwstl::swap(*iv1.begin(), *iv2.begin());
  zfwstl::for_each(iv1.begin(), iv1.end(), display<int>());
  zfwstl::for_each(iv2.begin(), iv2.end(), display<int>());

  string stra1[] = {"Jamie", "JJHou", "Jason"};
  string stra2[] = {"Jamie", "JJHou", "Jerry"};

  cout << zfwstl::lexicographical_compare(stra1, stra1 + 2, stra2, stra2 + 2);
  cout << zfwstl::lexicographical_compare(stra1, stra1 + 2, stra2, stra2 + 2,
                                          greater<string>());

  //=========================================================================
  /**
   * copy-overlap
   */
  // TAG: 如果copy区间有重叠==>std::memmove()会先将整个输入区间的内容复制下来，没有被覆盖的危险
  // {
  //   int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

  //   // 输出区间终点与输入区间重叠
  //   zfwstl::copy(ia + 2, ia + 7, ia);
  //   zfwstl::for_each(ia, ia + 9, display<int>());
  //   std::cout << std::endl;
  // }

  // {
  //   int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

  //   // 输出区间起点与输入区间重叠
  //   zfwstl::copy(ia + 2, ia + 7, ia + 4);
  //   zfwstl::for_each(ia, ia + 9, display<int>());
  //   std::cout << std::endl;
  //   // 本例正确，是因为copy使用memmove执行实际复制
  // }

  // {
  //   int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  //   zfwstl::deque<int> id(ia, ia + 9);

  //   auto first = id.begin();
  //   auto last = id.end();
  //   ++ ++first; // advance(first, 2);
  //   std::cout << *first << std::endl;
  //   -- --last;
  //   std::cout << *last << std::endl;

  //   auto result = id.begin();
  //   std::cout << *result << std::endl;

  //   // 输出区间终点与输入区间重叠
  //   zfwstl::copy(first, last, result);
  //   zfwstl::for_each(id.begin(), id.end(), display<int>());
  //   std::cout << std::endl;
  // }

  // {
  //   int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  //   zfwstl::deque<int> id(ia, ia + 9);

  //   auto first = id.begin();
  //   auto last = id.end();
  //   ++ ++first; // advance(first, 2);
  //   std::cout << *first << std::endl;
  //   -- --last;
  //   std::cout << *last << std::endl;

  //   auto result = id.begin();
  //   zfwstl::advance(result, 4);
  //   std::cout << *result << std::endl;

  //   // 输出区间起点与输入区间重叠
  //   zfwstl::copy(first, last, result);
  //   zfwstl::for_each(id.begin(), id.end(), display<int>());
  //   std::cout << std::endl;
  //   // 本例结果错误，是因为copy不再使用memmove执行实际复制

  //   // 如果换成vector结果正确，是因为vector迭代器时原生指针，
  //   // copy算法可以调用memmove执行实际复制
  // }

  //=========================================================================
  /**
   * copy-backward-overlap
   */
  // {
  //   int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

  //   // 输出区间终点与输入区间重叠
  //   zfwstl::copy_backward(ia + 2, ia + 7, ia + 9);
  //   zfwstl::for_each(ia, ia + 9, display<int>());
  //   std::cout << std::endl;
  // }

  // {
  //   int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

  //   // 输出区间起点与输入区间重叠
  //   zfwstl::copy_backward(ia + 2, ia + 7, ia + 5);
  //   zfwstl::for_each(ia, ia + 9, display<int>());
  //   std::cout << std::endl;
  //   // 本例正确，是因为copy_backward使用memmove执行实际复制
  // }

  // {
  //   int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  //   zfwstl::deque<int> id(ia, ia + 9);

  //   auto first = id.begin();
  //   auto last = id.end();
  //   ++ ++first; // advance(first, 2);
  //   std::cout << *first << std::endl;
  //   -- --last;
  //   std::cout << *last << std::endl;

  //   auto result = id.end();
  //   std::cout << *result << std::endl;

  //   // 输出区间终点与输入区间重叠
  //   zfwstl::copy_backward(first, last, result);
  //   zfwstl::for_each(id.begin(), id.end(), display<int>());
  //   std::cout << std::endl;
  // }

  // {
  //   int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  //   zfwstl::deque<int> id(ia, ia + 9);

  //   auto first = id.begin();
  //   auto last = id.end();
  //   ++ ++first; // advance(first, 2);
  //   std::cout << *first << std::endl;
  //   -- --last;
  //   std::cout << *last << std::endl;

  //   auto result = id.begin();
  //   zfwstl::advance(result, 5);
  //   std::cout << *result << std::endl;

  //   // 输出区间起点与输入区间重叠
  //   zfwstl::copy_backward(first, last, result);
  //   zfwstl::for_each(id.begin(), id.end(), display<int>());
  //   std::cout << std::endl;
  //   // 本例结果错误，是因为copy不再使用memmove执行实际复制

  //   // 如果换成vector结果正确，是因为vector迭代器时原生指针，
  //   // copy_backward算法可以调用memmove执行实际复制
  // }
}