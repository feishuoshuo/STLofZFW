#include "../src/algorithms/numeric.h"
#include "../STL/vector.h"
#include "../src/iterator.h" //for ostream_iterator
#include <functional>        // minus<int>()
#include <iostream>
#include <iterator>
#include <string>
int main()
{
  int ia[5] = {1, 2, 3, 4, 5};
  zfwstl::vector<int> iv(ia, ia + 5);
  // 0+1+2+3...
  std::cout << zfwstl::accumulate(iv.begin(), iv.end(), 0) << std::endl;

  // 0-1-2-3
  std::cout << zfwstl::accumulate(iv.begin(), iv.end(), 0, minus<int>()) << std::endl;

  // 10 + 1*1 + 2*2 + ...
  std::cout << zfwstl::inner_product(iv.begin(), iv.end(), iv.begin(), 10) << std::endl;

  // 10 - 1+1 - 2+2 - ...
  std::cout << zfwstl::inner_product(iv.begin(), iv.end(), iv.begin(), 10,
                                     minus<int>(), plus<int>())
            << std::endl;

  // 将迭代器绑定到cout，作为输出用
  zfwstl::ostream_iterator<int> oite(cout, " ");

  // 1 3 6 10 15 累计和
  zfwstl::partial_sum(iv.begin(), iv.end(), oite);
  std::cout << std::endl;

  // 1 -1 -4 -8 -13 累计差
  zfwstl::partial_sum(iv.begin(), iv.end(), oite, minus<int>());
  std::cout << std::endl;

  // 1 1 1 1 1     new #n = #n - #n-1
  zfwstl::adjacent_difference(iv.begin(), iv.end(), oite);
  std::cout << std::endl;

  // 1 3 5 7 9      new #n = op(#n, #n-1)
  zfwstl::adjacent_difference(iv.begin(), iv.end(), oite, plus<int>());
  std::cout << std::endl;

  // mingw c++ 中stl没有power实现
  std::cout << zfwstl::power(10, 3) << std::endl;
  std::cout << zfwstl::power(10, 3, zfwstl::plus<int>()) << std::endl;

  int n = 3;
  zfwstl::iota(iv.begin(), iv.end(), n); // 填入n, n+1, n+2
  for (int i = 0; i < iv.size(); ++i)
    std::cout << iv[i] << ' ';
  std::cout << std::endl;

  string avv = "abcde";
  string bvv = "hjklmn";
  auto x = zfwstl::lexicographical_compare(avv, bvv);
  std::cout << x << std::endl;
}