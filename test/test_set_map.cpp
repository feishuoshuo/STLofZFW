#include "../STL_2/set.h"
int main()
{
  /**
   * set map
   */
  int i;
  int ia[12] = {18, 25, 15, 6, 13, 37, 27, 17, 34, 9, 10, 23};
  zfwstl::set<int> iset{ia, ia + 5};

  // std::cout << "size=" << iset.size() << std::endl;
  // std::cout << "3 count =" << iset.count(3) << std::endl;
  iset.insert(3);
  // std::cout << "size=" << iset.size() << std::endl;
  // std::cout << "3 count =" << iset.count(3) << std::endl;

  iset.insert(5);
  // std::cout << "size=" << iset.size() << std::endl;
  // std::cout << "3 count =" << iset.count(3) << std::endl;
  zfwstl::set<int>::const_iterator ite1 = iset.begin();
  zfwstl::set<int>::const_iterator ite2 = iset.end();
  for (; ite1 != ite2; ++ite1)
  {
    std::cout << *ite1 << " ";
  }
  std::cout << std::endl;

  iset.erase(18);
  iset.erase(25);
  iset.erase(15);
  iset.erase(6);
  iset.erase(13);
  iset.erase(37);
  iset.erase(27);
  iset.erase(17);
  std::cout << "size=" << iset.size() << std::endl;
  std::cout << "3 count =" << iset.count(3) << std::endl;
  std::cout << "1 count =" << iset.count(3) << std::endl;

  ite1 = iset.begin();
  ite2 = iset.end();
  for (; ite1 != ite2; ++ite1)
  {
    std::cout << *ite1 << " ";
  }
  std::cout << std::endl;

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
  return 0;
}