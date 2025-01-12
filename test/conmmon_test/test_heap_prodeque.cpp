#include "../STL/priority_queue.h"
#include "../STL/vector.h"
#include "../STL/heap.h"
int main()
{
  /**
   * heap四个算法的试验
   */
  // int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
  // zfwstl::vector<int> ivec(ia, ia + 9);
  // for (int i = 0; i < ivec.size(); ++i)
  // {
  //   cout << ivec[i] << " ";
  // }
  // cout << endl;

  // zfwstl::make_heap(ivec.begin(), ivec.end());
  // for (int i = 0; i < ivec.size(); ++i)
  // {
  //   cout << ivec[i] << " ";
  // }
  // cout << endl;

  // ivec.push_back(7);
  // zfwstl::push_heap(ivec.begin(), ivec.end());
  // for (int i = 0; i < ivec.size(); ++i)
  // {
  //   cout << ivec[i] << " ";
  // }
  // cout << endl;

  // zfwstl::pop_heap(ivec.begin(), ivec.end());
  // cout << ivec.back() << endl;
  // ivec.pop_back();

  // for (int i = 0; i < ivec.size(); ++i)
  // {
  //   cout << ivec[i] << " ";
  // }
  // cout << endl;

  // zfwstl::sort_heap(ivec.begin(), ivec.end());
  // for (int i = 0; i < ivec.size(); ++i)
  // {
  //   cout << ivec[i] << " ";
  // }
  // cout << endl;
  /**
   * priority queue:双向开口的[分段]连续性空间
   */
  int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
  zfwstl::priority_queue<int> ipq(ia, ia + 9);
  cout << "size=" << ipq.size() << endl;

  for (int i = 0; i < ipq.size(); ++i)
    cout << ipq.top() << ' ';
  cout << endl;

  while (!ipq.empty())
  {
    cout << ipq.top() << ' ';
    ipq.pop();
  }
  cout << endl;
  return 0;
}