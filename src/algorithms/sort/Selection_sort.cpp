#include <iostream>
#include <utility>
#include <vector>
/**
 * 选择排序
 * 时间复杂度：O(n^2)
 * 空间复杂度：O(1)（原地）
 * 步骤：
 *    1.每次在数组中找min直接放到位置上
 */
void selection_sort(std::vector<int> &arr) {
  int n = arr.size();
  for (int i = 0; i < n - 1; ++i) {
    int min_index = i;
    for (int j = i + 1; j < n; ++j) {
      if (arr[min_index] > arr[j])
        min_index = j;
    }
    std::swap(arr[min_index], arr[i]);
  }
}

int main() {
  std::vector<int> arr = {4, 6, 8, 5, 9};
  selection_sort(arr);
  for (int x : arr)
    std::cout << x << ' ';
}