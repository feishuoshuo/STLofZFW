#include "../../STL/slist.h"
void pri(zfwstl::slist<int> &ilist)
{
  zfwstl::slist<int>::iterator ite;
  for (ite = ilist.begin(); ite != ilist.end(); ++ite)
  {
    std::cout << *ite << ' ';
  }
  std::cout << std::endl;
}
int main()
{
  /**
   * list:(带头结点的)循环双向链表
   * sort有三个版本[基数排序的变体、归并排序、快排]
   */
  zfwstl::slist<int> v4 = zfwstl::slist<int>({4, 5, 6});
  pri(v4);
  zfwstl::slist<int> v1, v2;
  v1 = {1, 2, 3, 4, 5};
  pri(v1);
  v2 = {6, 7, 8, 9, 10};
  pri(v2);
  zfwstl::slist<int> ilist;
  for (int j = 7; j >= 0; --j)
  {
    ilist.push_front(j);
  }
  pri(ilist);
  ilist.pop_front();
  ilist.pop_front();
  pri(ilist);
  // ilist.clear();
  // std::cout << ilist.size();
  return 0;
}