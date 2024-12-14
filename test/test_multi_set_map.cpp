#include "../STL_2/multiset.h"
int main()
{
  /**
   * multiset multimap
   */
  int i;
  int ia[12] = {18, 25, 15, 6, 13, 37, 27, 17, 34, 9, 10, 23};
  zfwstl::multiset<int> iset{ia, ia + 12};

  iset.insert(13);
  iset.insert(25);
  std::cout << "size=" << iset.size() << std::endl;
  std::cout << "3 count =" << iset.count(3) << std::endl;
  zfwstl::multiset<int>::const_iterator ite1 = iset.begin();
  zfwstl::multiset<int>::const_iterator ite2 = iset.end();

  for (; ite1 != ite2; ++ite1)
  {
    std::cout << *ite1 << " ";
  }
  std::cout << std::endl;
  return 0;
}