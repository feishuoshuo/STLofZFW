#include "../../STL/vector.h"
int main()
{
  /**
   * vector:
   * 刚开始创建容器大小不会给出多余空间，即finish = end_of_storage
   * 扩容：(原来容器大小*2) // const size_type len = old_size != 0 ? 2 * old_size : 1;
   */
  zfwstl::vector<int> v1 = {1, 2, 3, 4, 5};
  auto it = v1.crbegin();
  std::cout << *it << " ";

  zfwstl::vector<int> vec(4, 9); // 4 个元素，每个元素都是 9
  // zfwstl::vector<int> vec;
  std::cout << "size=" << vec.size() << std::endl;
  // std::cout << "max_size=" << vec.max_size() << std::endl;
  std::cout << "capacity=" << vec.capacity() << std::endl;
  // std::cout << "empty=" << vec.empty() << std::endl;
  std::cout << "end()=" << vec.back() << std::endl;
  vec.push_back(6);
  zfwstl::vector<int> vec33(zfwstl::move(vec));
  // 扩容的查看
  //  std::cout << "end()=" << vec.back() << std::endl;
  //  std::cout << "top=" << vec.front() << std::endl;
  //  std::cout << "size=" << vec.size() << std::endl;
  //  std::cout << "capacity=" << vec.capacity() << std::endl;

  auto p = vec33.data();
  vec33.insert(++p, 2, 10);
  for (size_t i = 0; i < vec33.size(); i++)
  {
    std::cout << vec33[i] << " ";
  }

  return 0;
}