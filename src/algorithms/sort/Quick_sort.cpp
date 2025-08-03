#include <iostream>
#include <utility>
#include <vector>
/**
 * 快排
 * 时间复杂度：O(n log n)
 * 空间复杂度：O(logn)（递归调用栈的深度）
 * 步骤：
 *    1.分区操作：通过一个pivot基准元素，将数组分成两部分(一部分小于基准；一部分大于基准)
 *    2.递归排序：对分区后的两部分分别递归地进行快速排序。
 */
int Partition(std::vector<int> &arr, int l, int r) {
  int p = r;
  --r;
  while (l <= r) {
    while (l <= r && arr[l] < arr[p])
      ++l;
    if (l <= r) {
      std::swap(arr[l], arr[p]);
      p = l;
      ++l;
    }
    while (l <= r && arr[r] >= arr[p])
      --r;
    if (l <= r) {
      std::swap(arr[r], arr[p]);
      p = r;
      --r;
    }
  }
  return p;
}
void quick_sort(std::vector<int> &arr, int low, int high) {
  if (low >= high)
    return;

  int p = Partition(arr, low, high);
  quick_sort(arr, low, p - 1);
  quick_sort(arr, p + 1, high);
}

int main() {
  std::vector<int> arr = {3, 5, 8, 1, 2, 9, 4, 7, 6};
  quick_sort(arr, 0, arr.size() - 1);
  for (int x : arr)
    std::cout << x << ' ';
}