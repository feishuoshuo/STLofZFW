#include <iostream>
#include <utility>
#include <vector>
/**
 * 冒泡排序
 * 时间复杂度：O(n^2)
 * 空间复杂度：O(1)（原地）
 * 步骤：
 *    1.每次都从下至上相邻元素对比交换
 *    2.特性：升序的最大元素一定在第一次冒泡中排好
 */
void bubble_sort(std::vector<int> &arr) {
  int n = arr.size();
  bool swapped;
  for (int i = 0; i < n - 1; ++i) {
    swapped = false;
    for (int j = 0; j < n - i - 1; ++j) {
      if (arr[j] > arr[j + 1]) {
        // 交换相邻元素
        std::swap(arr[j], arr[j + 1]);
        swapped = true;
      }
    }
    // 如果在某一轮中没有发生交换，说明数组已经排序完成
    if (!swapped) {
      break;
    }
  }
}

int main() {
  std::vector<int> arr = {4, 6, 8, 5, 9};
  bubble_sort(arr);
  for (int x : arr)
    std::cout << x << ' ';
}