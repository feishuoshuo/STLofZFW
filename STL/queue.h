#ifndef ZFWSTL_QUEUE_H_
#define ZFWSTL_QUEUE_H_
/**
 * 栈(FIFO 先进先出)
 * 属于container adapter
 */
#include <cstddef> //for size_t, ptrdiff_t
namespace zfwstl
{
  namespace zfwstl
  {
    template <class T /*,class Alloc=alloc*/>
    class deque
    {
    public:
      //  标准空间配置器
    public:
      // 构造、复制、移动、析构函数
      // TODO：需要完善
      //=================operator操作运算符重载=====================
      // 复制赋值操作符

      // 容量相关操作

      // 访问元素相关操作

      // 修改容器相关操作
    }
  }
}

#endif // !ZFWSTL_QUEUE_H_