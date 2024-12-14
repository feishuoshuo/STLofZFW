#include "../STL_2/set.h"
#include "../STL_2/map.h"
#include <iostream>
int main()
{
  /**
   * set map
   */
  // 自定义比较类，这里以忽略大小写的字符串比较为例

  zfwstl::map<std::string, int> simap;
  // simap.insert(std::make_pair(17, 1));
  // simap.insert(std::make_pair(18, 2));
  // simap.insert(std::make_pair(23, 3));
  // simap.insert(std::make_pair(34, 4));
  simap["c17"] = 1;
  simap["f18"] = 2;
  simap["e23"] = 3;
  simap["a34"] = 4;
  simap["e23"] = 10;
  // std::pair<int, int> value(10, 5);
  // simap.insert(value);
  zfwstl::map<std::string, int>::const_iterator ite1 = simap.begin();
  zfwstl::map<std::string, int>::const_iterator ite2 = simap.end();
  for (; ite1 != ite2; ++ite1)
  {
    std::cout << (*ite1).first << "=" << (*ite1).second << " ";
  }
  std::cout << std::endl;

  // zfwstl::map<string, int>::iterator simap_iter = simap.begin();
  // for (; simap_iter != simap.end(); ++simap_iter)
  //   cout << simap_iter->first << ' '
  //        << simap_iter->second << endl;

  // int number = simap[string("jjhou")];
  // cout << number << endl;

  // zfwstl::map<string, int>::iterator ite1;
  // // 对于关联式容器，专用find函数比STL算法find效率更高
  // ite1 = simap.find(string("mchen"));
  // if (ite1 == simap.end())
  //   cout << "mchen not found" << endl;

  // ite1 = simap.find(string("jerry"));
  // if (ite1 != simap.end())
  //   cout << "jerry found" << endl;

  // ite1->second = 9; // 可以修改value
  // int number2 = simap[string("jerry")];
  // cout << number2 << endl;

  return 0;
}

// int i;
// int ia[12] = {18, 25, 15, 6, 13, 37, 27, 17, 34, 9, 10, 23};
// zfwstl::set<int> iset{ia, ia + 12};

// std::cout << "size=" << iset.size() << std::endl;
// std::cout << "3 count =" << iset.count(3) << std::endl;
// iset.insert(3);
// std::cout << "size=" << iset.size() << std::endl;
// std::cout << "3 count =" << iset.count(3) << std::endl;

// iset.insert(5);
// std::cout << "size=" << iset.size() << std::endl;
// std::cout << "3 count =" << iset.count(3) << std::endl;
// zfwstl::set<int>::const_iterator ite1 = iset.begin();
// zfwstl::set<int>::const_iterator ite2 = iset.end();
// for (; ite1 != ite2; ++ite1)
// {
//   std::cout << *ite1 << " ";
// }
// std::cout << std::endl;

// iset.erase(18);
// iset.erase(25);
// iset.erase(15);
// iset.erase(6);
// iset.erase(13);
// iset.erase(37);
// iset.erase(27);
// iset.erase(17);
// std::cout << "size=" << iset.size() << std::endl;
// std::cout << "3 count =" << iset.count(3) << std::endl;
// std::cout << "1 count =" << iset.count(3) << std::endl;

// ite1 = iset.begin();
// ite2 = iset.end();
// for (; ite1 != ite2; ++ite1)
// {
//   std::cout << *ite1 << " ";
// }
// std::cout << std::endl;

// // 使用STL算法find可以搜索元素，但不推荐
// ite1 = zfwstl::find(iset.begin(), iset.end(), 3);
// if (ite1 != iset.end())
//   std::cout << "3 found" << std::endl;

// ite1 = zfwstl::find(iset.begin(), iset.end(), 1);
// if (ite1 == iset.end())
//   std::cout << "1 not found" << std::endl;

// // 关联式容器应使用专用的find函数搜索更有效率
// ite1 = iset.find(3);
// if (ite1 != iset.end())
//   std::cout << "3 found" << std::endl;

// ite1 = iset.find(1);
// if (ite1 == iset.end())
//   std::cout << "1 not found" << std::endl;

// *ite1 = 9; // 修改失败