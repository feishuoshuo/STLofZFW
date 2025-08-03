#include <iostream>
#include <vector>
/**
 * 插入排序
 * 时间复杂度：O(n^2)
 * 空间复杂度：O(1)（原地）
 * 步骤：
 *    1.在已排序序列中从后向前扫描，找到相应位置并插入
 */
void selection_sort(std::vector<int> &arr) {
  int n = arr.size();
  for (int i = 1; i < n - 1; ++i) {
    int key = arr[i]; // 选择未排序的第一个元素
    int j = i - 1;    // j负责从后向前找到已排序区域的key插入的下标位置

    // 将大于 key 的元素向后移动一个位置
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = key;
  }
}

int main() {
  std::vector<int> arr = {4, 6, 8, 5, 9};
  selection_sort(arr);
  for (int x : arr)
    std::cout << x << ' ';
}