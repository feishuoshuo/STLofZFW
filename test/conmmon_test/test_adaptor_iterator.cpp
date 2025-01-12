#include "../src/iterator.h" //for front_inserter, back_inserter, inserter, ostream_iterator, istream_iterator
#include <iostream>
#include "../STL/deque.h"
#include "../src/algorithms/algobase.h" //for copy
int main()
{
  /**
   * iterator-adapter
   */
  // 将outite绑定到cout，每次对outite指派一个元素，就后接一个“ ”
  zfwstl::ostream_iterator<int> outite(std::cout, " ");

  int ia[] = {0, 1, 2, 3, 4, 5};
  zfwstl::deque<int> id(ia, ia + 6);

  // // 将所有元素拷贝到outite，即cout
  // zfwstl::copy(id.begin(), id.end(), outite);
  // std::cout << std::endl;

  // // 将ia[]的部分元素拷贝到id内，使用front_insert_iterator
  // // front_insert_iterator会将assign操作给push_front操作
  // // vector不支持push_front操作，所以不以vector做示范对象
  // zfwstl::copy(ia + 1, ia + 2, zfwstl::front_inserter(id));
  // zfwstl::copy(id.begin(), id.end(), outite);
  // std::cout << std::endl;

  // // 将ia[]的部分元素拷贝到id内，使用back_insert_iterator
  // zfwstl::copy(ia + 1, ia + 2, zfwstl::back_inserter(id));
  // zfwstl::copy(id.begin(), id.end(), outite);
  // std::cout << std::endl;

  // // 搜寻元素5所在位置
  // zfwstl::deque<int>::iterator ite = zfwstl::find(id.begin(), id.end(), 5);
  // // 将ia[]的部分元素拷贝到id内，使用insert_iterator
  // zfwstl::copy(ia + 1, ia + 2, zfwstl::inserter(id, ite));
  // zfwstl::copy(id.begin(), id.end(), outite);
  // std::cout << std::endl;
  //===========================================================================
  // 将所有元素逆向拷贝到outite
  // rbegin()和rend()与reverse_iterator有关
  zfwstl::copy(id.rbegin(), id.rend(), outite);
  std::cout << std::endl;

  // 将inite绑定到cin，将元素拷贝到inite，知道eos出现
  zfwstl::istream_iterator<int> inite(cin), eos; // eos: end-of-stream
  zfwstl::copy(inite, eos, zfwstl::inserter(id, id.begin()));
  // 输入数字，停止时可以输入任意字符

  zfwstl::copy(id.begin(), id.end(), outite);
  return 0;
}