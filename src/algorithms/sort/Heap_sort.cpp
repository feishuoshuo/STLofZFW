#include <iostream>
#include <utility>
#include <vector>

/**
 * 大根堆(升序排序)[利用完全二叉树性质]
 * 时间复杂度：O(n log n)
 * 空间复杂度：O(1)（原地）
 * 步骤：
 *    1.建立大根堆：从(末尾)叶子节点，左->右，上->下 调整
 *    2.将堆顶元素与尾部元素交换(调整len长度)，使得尾部元素最大
 */
void heapify(std::vector<int> &arr, int index, int len) {
  int root = index;
  while (true) {
    // 左右孩子节点下标
    int left = index * 2 + 1, right = index * 2 + 2;
    // 判断左右孩子值是否大于根
    if (left < len && arr[left] > arr[root])
      root = left;
    if (right < len && arr[right] > arr[root])
      root = right;

    if (root == index)
      break;
    // 若最大不是根，则继续下沉
    std::swap(arr[root], arr[index]);
    index = root;
  }
}
void heap_sort(std::vector<int> &arr) {
  int n = arr.size();
  if (n < 2)
    return;
  // 1-建立大根堆
  for (int i = n / 2 - 1; i >= 0; --i)
    heapify(arr, i, n);

  // 2-堆顶元素与尾部元素交换
  for (int end = n - 1; end > 0; --end) {
    std::swap(arr[0], arr[end]); // 当前最大值归位
    heapify(arr, 0, end);        // 对剩余 [0, end) 区间重新调整为最大堆
  }
}

int main() {
  /**
        4
       / \
     6    8
    / \
   5   9
  */
  std::vector<int> arr = {4, 6, 8, 5, 9};
  heap_sort(arr);
  for (int x : arr)
    std::cout << x << ' ';
}