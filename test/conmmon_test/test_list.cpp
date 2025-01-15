#include "../../STL/list.h"
#include "../../src/iterator.h" //for advance
void pri(zfwstl::list<int> ilist)
{
  zfwstl::list<int>::iterator ite;
  for (ite = ilist.begin(); ite != ilist.end(); ++ite)
  {
    std::cout << *ite << ' ';
  }
  cout << std::endl;
}
int main()
{
  /**
   * list:(带头结点的)循环双向链表
   * sort有三个版本[基数排序的变体、归并排序、快排]
   */
  {
    zfwstl::list<int> lst = {5, 2, 8, 3, 1, 6};
    lst.sort([](int a, int b)
             { return a < b; }, '\0');
    std::cout << (lst == zfwstl::list<int>({1, 2, 3, 5, 6, 8})) << std::endl;

    lst = {5, 2, 8, 3, 1, 6};
    lst.sort([](int a, int b)
             { return a < b; }, 'q');
    std::cout << (lst == zfwstl::list<int>({1, 2, 3, 5, 6, 8})) << std::endl;

    lst = {5, 2, 8, 3, 1, 6};
    lst.sort([](int a, int b)
             { return a < b; }, 'm');
    std::cout << (lst == zfwstl::list<int>({1, 2, 3, 5, 6, 8})) << std::endl;
  }

  int i;
  zfwstl::list<int> ilist;
  for (int j = 7; j >= 0; --j)
  {
    ilist.push_back(j);
  }
  pri(ilist);
  ilist.pop_back();
  pri(ilist);
  // ilist.sort(); // q m 空
  // pri(ilist);
  // std::cout << "size=" << ilist.size() << std::endl;

  // ite = find(ilist.begin(), ilist.end(), 3);
  // if (ite != ilist.end())
  // {
  //   ilist.insert(ite, 99);
  // }
  // std::cout << "size=" << ilist.size() << std::endl;
  // std::cout << *ite << std::endl;

  // for (ite = ilist.begin(); ite != ilist.end(); ++ite)
  // {
  //   std::cout << *ite << ' ';
  // }
  // std::cout << std::endl;

  // ite = find(ilist.begin(), ilist.end(), 1);
  // if (ite != ilist.end())
  // {
  //   std::cout << *(ilist.erase(ite)) << std::endl;
  // }

  // for (ite = ilist.begin(); ite != ilist.end(); ++ite)
  // {
  //   std::cout << *ite << ' ';
  // }
  // std::cout << std::endl;

  // int iv[5] = {5, 6, 7, 8, 9};
  // zfwstl::list<int> ilist2(iv, iv + 5);
  // // 目前，ilist的内容为 0 2 99 3 4
  // ite = find(ilist.begin(), ilist.end(), 99);
  // ilist.splice(ite, ilist2);
  // ilist.reverse();
  // ilist.sort();

  return 0;
}