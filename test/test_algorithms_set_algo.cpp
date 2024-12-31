#include "../STL_2/multiset.h"
#include "../src/iterator.h"            // for ostream_iterator
#include "../src/algorithms/algo.h"     // for for_each
#include "../src/algorithms/set_algo.h" // for set_union, set_intersection, set_symmetric_difference
#include <iostream>
template <class T>
struct display
{
  void operator()(const T &x)
  {
    cout << x << ' ';
  }
};

int main()
{
  int ia1[6] = {1, 3, 5, 7, 9, 11};
  int ia2[7] = {1, 1, 2, 3, 5, 8, 13};

  zfwstl::multiset<int> S1(ia1, ia1 + 6);
  zfwstl::multiset<int> S2(ia2, ia2 + 7);

  zfwstl::for_each(S1.begin(), S1.end(), display<int>());
  std::cout << std::endl;
  zfwstl::for_each(S2.begin(), S2.end(), display<int>());
  std::cout << std::endl;

  auto first1 = S1.begin();
  auto last1 = S1.end();

  auto first2 = S2.begin();
  auto last2 = S2.end();

  cout << "Union of S1 and S2: ";
  zfwstl::set_union(first1, last1, first2, last2, zfwstl::ostream_iterator<int>(cout, " "));
  std::cout << std::endl;

  first1 = S1.begin();
  first2 = S2.begin();
  cout << "Intersection of S1 and S2: ";
  zfwstl::set_intersection(first1, last1, first2, last2, zfwstl::ostream_iterator<int>(cout, " "));
  std::cout << std::endl;

  first1 = S1.begin();
  first2 = S2.begin();
  cout << "Difference of S1 and S2: ";
  zfwstl::set_difference(first1, last1, first2, last2, zfwstl::ostream_iterator<int>(cout, " "));
  cout << endl;

  first1 = S1.begin();
  first2 = S2.begin();
  cout << "Symmetric difference of S1 and S2: ";
  zfwstl::set_symmetric_difference(first1, last1, first2, last2, zfwstl::ostream_iterator<int>(cout, " "));
  cout << endl;
}