#include <iostream>
#include <vector>
/**
 * 希尔排序/缩小增量排序: 是插入排序的一种改进版本
 * 时间复杂度：O(n(log n)^2)
 * 空间复杂度：O(1)（原地）
 * 步骤：
 *    1.注意它比较的是gap距离的两个元素
      2.在已排序序列中从后向前扫描，找到相应位置并插入
 */
void shell_sort(std::vector<int> &arr) {
  int n = arr.size();
  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; ++i) {
      int tmp = arr[i];
      int j;
      for (j = i; j >= gap && arr[j - gap] > tmp; j -= gap) {
        arr[j] = arr[j - gap];
      }
      arr[j] = tmp;
    }
  }
}

int main() {
  std::vector<int> arr = {12, 34, 54, 2, 3};
  shell_sort(arr);
  for (int x : arr)
    std::cout << x << ' ';
}