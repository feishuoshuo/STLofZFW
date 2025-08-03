#include <algorithm>
#include <iostream>
#include <vector>
/**
 * 基数排序
 * 时间复杂度：O(n * k)
 * 空间复杂度：O(n + k)
 * 步骤：
 *    1.特性：每个阶段按照数字的某一位进行排序，从最低位开始，逐步向上直到最高位
 */
void countingSortByDigit(std::vector<int> &arr, int exp) {
  std::vector<int> output(arr.size());
  int count[10] = {0};
  for (int i = 0; i < arr.size(); i++) {
    count[(arr[i] / exp) % 10]++;
  }
  for (int i = 1; i < 10; i++) {
    count[i] += count[i - 1];
  }
  for (int i = arr.size() - 1; i >= 0; i--) {
    output[count[(arr[i] / exp) % 10] - 1] = arr[i];
    count[(arr[i] / exp) % 10]--;
  }
  for (int i = 0; i < arr.size(); i++) {
    arr[i] = output[i];
  }
}

void radix_sort(std::vector<int> &arr) {
  int maxVal = *std::max_element(arr.begin(), arr.end());
  for (int exp = 1; maxVal / exp > 0; exp *= 10) {
    countingSortByDigit(arr, exp);
  }
}

int main() {
  std::vector<int> arr = {170, 45, 75, 90, 802, 24, 2, 66};
  radix_sort(arr);
  for (int x : arr)
    std::cout << x << ' ';
}