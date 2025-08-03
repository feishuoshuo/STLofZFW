#include <algorithm>
#include <iostream>
#include <vector>
/**
 * 桶排序
 * 时间复杂度：O(n + k)
 * 空间复杂度：O(n + k)
 * 步骤：
 *    1.分桶：根据数据范围和桶的数量，将数组元素分配到不同的桶中。
 *    2.桶内排序：对每个桶内的元素进行排序，可以使用其他简单的排序算法（如插入排序）
 *    3.合并：将所有桶中的元素按顺序合并，形成最终的有序序列
 */
void bucket_sort(std::vector<int> &arr) {
  if (arr.empty())
    return; // 如果数组为空，直接返回

  // 找到数组中的最小值和最大值
  int minVal = *std::min_element(arr.begin(), arr.end());
  int maxVal = *std::max_element(arr.begin(), arr.end());

  // 确定桶的数量和每个桶的范围
  int bucketCount = maxVal - minVal + 1;
  std::vector<std::vector<int>> buckets(bucketCount);

  // 将每个元素分配到对应的桶中
  for (int num : arr) {
    int bucketIndex = num - minVal;
    buckets[bucketIndex].push_back(num);
  }

  // 对每个桶内的元素进行排序
  for (auto &bucket : buckets) {
    std::sort(bucket.begin(), bucket.end());
  }

  // 合并所有桶中的元素
  int index = 0;
  for (auto &bucket : buckets) {
    for (int num : bucket) {
      arr[index++] = num;
    }
  }
}

int main() {
  std::vector<int> arr = {34, 2, 23, 67, 100, 88, 45, 22, 28, 90, 12, 76};
  bucket_sort(arr);
  for (int x : arr)
    std::cout << x << ' ';
}