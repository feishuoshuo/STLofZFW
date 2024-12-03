#include "../STL/deque.h"
int main()
{
  /**
   * deque:双向开口的[分段]连续性空间
   */
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
  return 0;
}