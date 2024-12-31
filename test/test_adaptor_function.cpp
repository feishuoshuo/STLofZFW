#include "../src/iterator.h" //for ostream_iterator, istream_iterator
#include "../STL/vector.h"
#include <iostream>
int main()
{
  /**
   * function-adapter
   */
  // 将outite绑定到cout，每次对outite指派一个元素，就后接一个“ ”
  zfwstl::ostream_iterator<int> outite(std::cout, " ");

  int ia[6] = {2, 21, 12, 7, 19, 23};
  zfwstl::vector<int> iv(ia, ia + 6);

  // 找出不小于12的元素个数
  std::cout << zfwstl::count_if(iv.begin(), iv.end(),
                                not1(bind2nd(less<int>(), 12)));
  std::cout << std::endl;

  // 令每个元素v执行(v+2)*3然后输往outite
  transform(iv.begin(), iv.end(), outite,
            compose1(bind2nd(multiplies<int>(), 3),
                     bind2nd(plus<int>(), 2)));
  std::cout << std::endl;

  // 将所有元素拷贝到outite
  copy(iv.begin(), iv.end(), outite);
  std::cout << std::endl;
  // 1. 使用函数指针搭配stl算法
  for_each(iv.begin(), iv.end(), print);
  std::cout << std::endl;

  // 2. 以修饰过的一般函数搭配stl算法
  for_each(iv.begin(), iv.end(), ptr_fun(print));
  std::cout << std::endl;

  Int t1(3), t2(7), t3(20), t4(14), t5(68);
  vector<Int> Iv;
  Iv.push_back(t1);
  Iv.push_back(t2);
  Iv.push_back(t3);
  Iv.push_back(t4);
  Iv.push_back(t5);
  // 3. 以修饰过的成员函数搭配stl算法
  for_each(Iv.begin(), Iv.end(), mem_fun_ref(&Int::print1));
  return 0;
}