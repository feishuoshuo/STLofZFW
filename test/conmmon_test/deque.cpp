#include "../../STL/deque.h"
void pri(zfwstl::deque<int, 64> ilist)
{
  for (auto ite = ilist.begin(); ite != ilist.end(); ++ite)
  {
    std::cout << *ite << ' ';
  }
  cout << std::endl;
}
int main()
{
  /**
   * deque:双向开口的[分段]连续性空间
   */
  // {
  //   zfwstl::deque<int, 8> d(100, 3);
  //   d.resize(50);
  //   d.shrink_to_fit();
  // }
  zfwstl::deque<int> ideq(20, 9);
  std::cout << "size=" << ideq.size() << std::endl;

  for (int i = 0; i < ideq.size(); ++i)
    std::cout << ideq[i] << ' ';
  std::cout << std::endl;

  for (int i = 0; i < 3; ++i)
    ideq.push_back(i);

  for (int i = 0; i < ideq.size(); ++i)
    std::cout << ideq[i] << ' ';
  std::cout << std::endl;
  std::cout << "size=" << ideq.size() << std::endl;

  ideq.push_back(3);
  for (int i = 0; i < ideq.size(); ++i)
    std::cout << ideq[i] << ' ';
  std::cout << std::endl;
  std::cout << "size=" << ideq.size() << std::endl;

  ideq.push_front(99);
  for (int i = 0; i < ideq.size(); ++i)
    std::cout << ideq[i] << ' ';
  std::cout << std::endl;
  std::cout << "size=" << ideq.size() << std::endl;

  ideq.push_front(98);
  ideq.push_front(97);
  for (int i = 0; i < ideq.size(); ++i)
    std::cout << ideq[i] << ' ';
  std::cout << std::endl;
  std::cout << "size=" << ideq.size() << std::endl;

  // zfwstl::deque<int>::iterator itr;
  // itr = find(ideq.begin(), ideq.end(), 99);
  // std::cout << *itr << endl;
  // std::cout << *(itr.cur) << std::endl;

  /**
   * 测试reverse_iterator
   */
  zfwstl::deque<int>::reverse_iterator rite2(ideq.end());
  std::cout << *(rite2) << std::endl;
  std::cout << *(++ ++ ++rite2) << std::endl;
  std::cout << *(--rite2) << std::endl;
  std::cout << *(rite2.base()) << std::endl;
  std::cout << rite2[3] << std::endl;
  return 0;
}