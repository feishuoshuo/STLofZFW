#include <algorithm>
#include <iostream>
#include <vector>
/**
 * 计数排序
 * 时间复杂度：O(n + k)
 * 空间复杂度：O(k)
 * 步骤：
 *    1.利用一个额外的数组（称为“计数数组”）来统计每个整数出现的次数
 *    2.然后通过计数数组来确定每个整数在排序后数组中的位置
 */
void counting_sort(std::vector<int> &arr) {
  // 找到数组中的最大值和最小值
  int maxVal = *std::max_element(arr.begin(), arr.end());
  int minVal = *std::min_element(arr.begin(), arr.end());
  int range = maxVal - minVal;

  // 创建计数数组
  std::vector<int> countArray(range + 1, 0);
  // 计算每个元素出现的次数
  for (int num : arr) {
    countArray[num - minVal]++;
  }

  // 累加计数数组的作用是帮助我们确定每个元素在最终排序数组中的确切位置，而不是简单地统计每个元素出现的次数
  for (int i = 1; i < countArray.size(); ++i) {
    countArray[i] += countArray[i - 1];
  }

  // 创建输出数组
  std::vector<int> output(arr.size());

  // 将元素放到正确的位置
  for (int i = arr.size() - 1; i >= 0; --i) {
    output[countArray[arr[i] - minVal] - 1] = arr[i];
    countArray[arr[i] - minVal]--;
  }

  // 将排序后的结果复制回原数组
  arr = output;
}

int main() {
  std::vector<int> arr = {4, 6, 8, 8, 8, 8, 5, 9, 5, 9};
  counting_sort(arr);
  for (int x : arr)
    std::cout << x << ' ';
}