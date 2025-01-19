#include "../../STL_2/multimap.h"
#include "../../STL_2/multiset.h"
#include "../../src/util.h"
#include <iostream>
#include <string>
int main()
{
  /**
   * multiset multimap
   */
  zfwstl::multimap<std::string, int> simap;
  simap.insert(zfwstl::make_pair(std::string("c5517"), 1));
  simap.insert(zfwstl::make_pair(std::string("f16678"), 2));
  simap.insert(zfwstl::make_pair(std::string("e8323"), 3));
  auto res = simap.insert(zfwstl::make_pair(std::string("a34"), 4));
  auto jj = (*(res)).second;
  simap["c17"] = 1;
  simap["f18"] = 2;
  simap["e23"] = 7;
  simap["a34"] = 4;
  simap["e23"] = 10;
  auto j = *(simap.find("e23"));

  // std::pair<int, int> value(10, 5);
  // simap.insert(value);
  // string arr[] = {"c1", "a2", "v4", "o8", "@10"};
  // for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
  //   simap[arr[i]] = i;
  // // zfwstl::multimap<int, std::string> myMap({{1, "one"}, {2, "two"}, {3, "three"}});
  auto it = simap.find("c1");
  // if (it != simap.end())
  // {
  //   std::cout << it->second << std::endl; // 输出 "one"
  // }
  // else
  // {
  //   std::cout << "Key not found" << std::endl;
  // }

  // zfwstl::multimap<int, std::string>::const_iterator ite1 = myMap.begin();
  // zfwstl::multimap<int, std::string>::const_iterator ite2 = myMap.end();
  // for (; ite1 != ite2; ++ite1)
  // {
  //   std::cout << (*ite1).first << "=" << (*ite1).second << " ";
  // }
  // std::cout << std::endl;

  // int i;
  // int ia[12] = {18, 25, 15, 6, 13, 37, 27, 17, 34, 9, 10, 23};
  // zfwstl::multiset<int> iset{ia, ia + 12};

  // iset.insert(13);
  // iset.insert(25);
  // std::cout << "size=" << iset.size() << std::endl;
  // std::cout << "3 count =" << iset.count(3) << std::endl;
  // zfwstl::multiset<int>::const_iterator ite3 = iset.begin();
  // zfwstl::multiset<int>::const_iterator ite4 = iset.end();

  // for (; ite3 != ite4; ++ite3)
  // {
  //   std::cout << *ite3 << " ";
  // }
  // std::cout << std::endl;
  return 0;
}