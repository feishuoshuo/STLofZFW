#include "../STL/stack.h"
#include "../STL/queue.h"
int main()
{
  /**
   * deque:双向开口的[分段]连续性空间
   */
  zfwstl::queue<int> sta; // stack, queue
  sta.push(5);
  sta.push(52);
  sta.push(11);
  std::cout << "size=" << sta.size() << std::endl;
  std::cout << sta.top() << std::endl;
  sta.pop();
  std::cout << sta.top() << std::endl;

  return 0;
}