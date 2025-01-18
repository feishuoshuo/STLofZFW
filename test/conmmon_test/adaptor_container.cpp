#include "../../STL/stack.h"
#include "../../STL/queue.h"
int main()
{
  /**
   * deque:双向开口的[分段]连续性空间
   */
  zfwstl::stack<int> v1, v2;
  v1 = {1, 2, 3, 4, 5};
  v2 = {6, 7, 8, 9, 10};
  zfwstl::stack<int> v6(zfwstl::move(v2));
  std::cout << (v1 != v2) << std::endl;
  std::cout << (v1 == v2) << std::endl;
  std::cout << (v1 <= v2) << std::endl;
  std::cout << (v1 >= v2) << std::endl;
  std::cout << (v1 < v2) << std::endl;
  std::cout << (v1 > v2) << std::endl;
  return 0;
}