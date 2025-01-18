#include "../STL_2/multimap.h"
#include "../STL_2/multiset.h"
#include <iostream>
#include <string>
int main()
{
  /**
   * multiset multimap
   */
  // zfwstl::multimap<std::string, int> simap;
  // simap.insert(std::make_pair("c17", 1));
  // simap.insert(std::make_pair("f18", 2));
  // simap.insert(std::make_pair("e23", 3));
  // simap.insert(std::make_pair("a34", 4));
  // simap["c17"] = 1;
  // simap["f18"] = 2;
  // simap["e23"] = 7;
  // simap["a34"] = 4;
  // simap["e23"] = 10;
  // // std::pair<int, int> value(10, 5);
  // // simap.insert(value);
  // zfwstl::multimap<std::string, int>::const_iterator ite1 = simap.begin();
  // zfwstl::multimap<std::string, int>::const_iterator ite2 = simap.end();
  // for (; ite1 != ite2; ++ite1)
  // {
  //   std::cout << (*ite1).first << "=" << (*ite1).second << " ";
  // }
  // std::cout << std::endl;

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