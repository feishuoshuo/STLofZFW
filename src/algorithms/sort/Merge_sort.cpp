#include <iostream>
#include <ostream>
#include <vector>
/**
 * 归并排序
 * 时间复杂度：O(nlogn)
 * 空间复杂度：O(n)
 * 步骤：
 *    1.每次都从下至上相邻元素对比交换
 *    2.特性：升序的最大元素一定在第一次冒泡中排好
 */
void merge(std::vector<int> &arr, int l, int mid, int r) {
  int n1 = mid - l + 1; // 左半部分的大小
  int n2 = r - mid;     // 右半部分的大小

  // 创建临时数组
  std::vector<int> L(n1);
  std::vector<int> R(n2);

  // 复制数据到临时数组
  for (int i = 0; i < n1; ++i) {
    L[i] = arr[l + i];
  }
  for (int j = 0; j < n2; ++j) {
    R[j] = arr[mid + 1 + j];
  }
  int M_index = l, l1 = 0, l2 = 0;

  while (l1 < L.size() && l2 < R.size()) {
    if (L[l1] < R[l2]) {
      arr[M_index] = L[l1];
      ++l1;
    } else {
      arr[M_index] = R[l2];
      ++l2;
    }
    ++M_index;
  }
  while (l1 < L.size()) {
    arr[M_index] = L[l1];
    ++l1;
    ++M_index;
  }
  while (l2 < R.size()) {
    arr[M_index] = R[l2];
    ++l2;
    ++M_index;
  }
}
void merge_sort(std::vector<int> &arr, int left, int right) {
  if (left >= right)
    return;

  int middle = left + (right - left) / 2;
  merge_sort(arr, left, middle);
  merge_sort(arr, middle + 1, right);

  merge(arr, left, middle, right);
}

int main() {
  std::vector<int> arr = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  merge_sort(arr, 0, arr.size() - 1);
  for (int x : arr)
    std::cout << x << ' ';
}