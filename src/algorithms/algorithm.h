#ifndef ZFWSTL_ALGORITHM_H_
#define ZFWSTL_ALGORITHM_H_
/**
 * 这个头文件包含了 mystl 的所有算法，包括基本算法，数值算法，heap 算法，set 算法和其他算法
 * - 所有STL算法都作用在由迭代器[first, last)所标识出来的区间上
 *    - 质变算法：
 *        如拷贝copy, 互换swap, 替换replace, 填写fill,
 *         删除remove, 排列组合permutation, 分割partition,
 *         随机重排random shuffling, 排序sort等
 *    - 非质变算法：
 *        如查找find, 匹配search, 计数count, 巡防for_each,
 *         比较equal/mismatch, 寻找极值max/min等
 */
#include "algobase.h"
#include "algo.h"
#include "set_algo.h"
#include "heap_algo.h"
#include "numeric.h"
/**
 * - _if结尾, 可接收仿函数进行比对行为
 * _copy结尾，表示将操作对象内容复制一份副本，然后在副本上进行下一步操作
 * _n结尾，n次/个
 */
namespace zfwstl
{

}

#endif // !ZFWSTL_ALGORITHM_H_
