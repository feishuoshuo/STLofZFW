#ifndef ZFWSTL_ALLOC_H_
#define ZFWSTL_ALLOC_H_
/**
 * 从 v2.0.0 版本开始，将不再使用内存池，这个文件将被弃用，但暂时保留
 * 推荐使用 <stdlib.h>（C）或 <cstdlib>（C++）来替代 alloc.h 头文件
 *
 * 特殊的空间配置器：内存空间的配置与释放
 * 配备:
 * 一级配置器_malloc_alloc_template
 * 二级配置器__default_alloc_template 内存池，自由链表free_list
 */
#include <cstddef> // for size_t, ptrdiff_t, nullptr_t
#include <cstdlib> // for malloc(), free(), realloc(), abort
#include <new>     // for std::bad_alloc
#include <iostream>
#include <cstring> // for memcpy
namespace zfwstl
{
  void global_oom_handler()
  {
    std::cerr << "Out of Memory! Taking emergency actions..." << std::endl;
    // 这里可以添加一些紧急处理措施，比如尝试释放缓存，或者减少内存使用等
    std::abort();
  }

  // 1. 一级配置器
  class _malloc_alloc_template
  {
  private:
    //  oom: out of memory
    static void *oom_malloc(size_t n)
    {
      void *ptr = std::malloc(n);
      if (ptr == nullptr)
      { // 内存分配失败
        if (__malloc_alloc_oom_handler)
        {
          std::cout << "oom: malloc error!!" << std::endl;
          __malloc_alloc_oom_handler(); // 调用 OOM 处理函数
        }
        throw std::bad_alloc(); // 抛出异常
      }
      return ptr;
    }
    static void *oom_realloc(void *ptr, size_t new_size)
    {
      void *new_ptr = std::realloc(ptr, new_size);
      if (new_ptr == nullptr)
      { // 内存分配失败
        if (__malloc_alloc_oom_handler)
        {
          std::cout << "oom: realloc error!!" << std::endl;
          __malloc_alloc_oom_handler(); // 调用 OOM 处理函数
        }
        throw std::bad_alloc(); // 抛出异常
      }
      return new_ptr;
    }
    static void (*__malloc_alloc_oom_handler)(); // 函数指针，所代表的函数将用来处理内存不足情况

  public:
    static void *allocate(size_t n)
    {
      void *result = std::malloc(n); // 直接调用 std::malloc分配内存
      // 以下无法满足需求时，改用oom_malloc()
      if (0 == result)
        result = oom_malloc(n);
      return result;
    }
    static void deallocate(void *_p, size_t /* n */)
    {
      free(_p); // 直接调用 std::free释放内存
    }
    static void *reallocate(void *_p, size_t /* old_sz */, size_t new_sz)
    {
      void *result = std::realloc(_p, new_sz); // 直接调用 std::realloc重新内存
      // 以下无法满足需求时，改用oom_realloc()
      if (0 == result)
        result = oom_realloc(_p, new_sz);
      return result;
    }
  };

  void (*_malloc_alloc_template::__malloc_alloc_oom_handler)() = global_oom_handler;
  // 2. 二级配置器： 为了避免太多小额区块造成的内存碎片
  /**
   * 当内存较小时，以内存池管理，每次配置一大块内存，并维护对应的自由链表
   * 如果内存较大，超过 128 bytes，使用一级配置器[即直接调用 std::malloc, std::free]
   */
  // 共用体: FreeList
  // 采用链表的方式管理内存碎片，分配与回收小内存（<=4K）区块
  // TAG: union 允许在相同的内存位置存储不同的数据类型
  union FreeList // free_list的节点构造
  {
    union FreeList *next; // 指向下一个区块
    char client_data[1];  // 储存本块内存的首地址
  };
  // 不同内存范围的上调大小
  enum // "the enum hack"补偿做法
  {
    __ALIGN = 8
  };
  // 小对象的内存大小
  enum
  {
    __MAX_BYTES = 128
  };
  // free lists 个数
  enum // "the enum hack"补偿做法
  {
    __FREELISTNUM = __MAX_BYTES / __ALIGN
  };

  class __default_alloc_template
  {
  private:
    // TAG: volatile 告诉编译器该变量值可能会在程序控制之外随时变化，每次都需重新从内存中读取
    static FreeList *free_list[__FREELISTNUM]; // 16个自由链表

    // 内存池 memory pool
    static char *start_free; // 内存池起始位置
    static char *end_free;   // 内存池结束位置
    static size_t heap_size; // 申请 heap 空间附加值大小

  private:
    // 将bytes上调至__ALIGN = 8的倍数
    static size_t ROUND_UP(size_t bytes)
    {
      return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1));
    }
    // 根据区块大小，选择第 n 个 free lists
    static size_t FREELIST_INDEX(size_t bytes)
    {
      return (((bytes) + __ALIGN - 1) / __ALIGN - 1);
    }
    // 重新填充 free list
    // 返回一个大小为 n 的对象，并可能加入大小为 n 的其他区块到 free_list
    // 缺省取得20个新节点(新区块)，但万一内存池空间不足，获得的节点数可能 < 20
    static void *refill(size_t n)
    {
      size_t nobj = 20;
      char *chunk = chunk_alloc(n, nobj);
      FreeList *volatile *my_free_list;
      FreeList *result, *cur, *next;
      // 如果只有一个区块，就把这个区块返回给调用者，free list 没有增加新节点
      if (1 == nobj)
        return chunk;
      // 否则把一个区块给调用者，剩下的纳入 free list 作为新节点
      my_free_list = free_list + FREELIST_INDEX(n);
      result = (FreeList *)chunk; // 这一块返回给客端
      *my_free_list = next = (FreeList *)(chunk + n);
      // 将free_list的各个节点串接起来
      for (size_t i = 1;; ++i) //= 1 是因为第 0 个给了客端
      {
        cur = next;
        next = (FreeList *)((char *)next + n);
        if (nobj - 1 == i)
        {
          cur->next = nullptr;
          break;
        }
        else
        {
          cur->next = next;
        }
      }
      return result;
    }
    // 从内存池中取一大块空间，可容纳 nobj 个大小为 “size” 的区块
    // 条件不允许时，会调整 nblock
    /**
     * 这里char*是因为 char类型的大小是 1 字节，
     * 这意味着 char 数组的起始地址可以被用作任何其他类型对象的起始地址，而不需要额外的内存对齐操作
     * 在某些内存分配策略中，使用 char 数组可以更有效地利用内存，减少内存碎片
     */
    static char *chunk_alloc(size_t size, size_t &nobj)
    {
      char *result;
      size_t need_bytes = size * nobj;           // need_bytes 需求量
      size_t pool_bytes = end_free - start_free; // pool_bytes 内存池剩余空间

      // 如果内存池剩余大小完全满足需求量，返回它
      if (pool_bytes >= need_bytes)
      {
        result = start_free;
        start_free += need_bytes;
        return result;
      }

      // 如果内存池剩余大小不能完全满足需求量，但至少可以分配一个或一个以上的区块，就返回它
      else if (pool_bytes >= size)
      {
        nobj = pool_bytes / size;
        need_bytes = size * nobj;
        result = start_free;
        start_free += need_bytes;
        return result;
      }

      // 如果内存池剩余大小连一个区块都无法满足
      else
      {
        // 将内存池中的残余零头利用起来
        if (pool_bytes > 0)
        { // 如果内存池还有剩余，把剩余的空间加入到 free list 中
          FreeList *volatile *my_free_list = free_list + FREELIST_INDEX(pool_bytes);
          // 头插法
          ((FreeList *)start_free)->next = *my_free_list;
          *my_free_list = (FreeList *)start_free;
        }
        // 申请 heap 空间，用来补充内存池
        size_t bytes_to_get = (need_bytes << 1) + ROUND_UP(heap_size >> 4);
        start_free = (char *)std::malloc(bytes_to_get);
        // TODO: 内存池无法满足需求-->转用一级配置器
        if (!start_free)
        { // NOTE: heap 空间也不够，它就去free list当中找空闲内存填内存池[相当于回收了]
          FreeList *volatile *my_free_list, *p;
          // 试着查找有无未用区块，且区块足够大的 free list
          for (size_t i = size; i <= __MAX_BYTES; i += __ALIGN)
          {
            my_free_list = free_list + FREELIST_INDEX(i);
            p = *my_free_list;
            if (p)
            {
              *my_free_list = p->next;
              start_free = (char *)p;
              end_free = start_free + i;
              // 递归调用自己，为了修正nobj
              return chunk_alloc(size, nobj);
            }
          }
          std::printf("out of memory");
          end_free = nullptr;
          throw std::bad_alloc();
        }
        end_free = start_free + bytes_to_get;
        heap_size += bytes_to_get;
        // 递归调用自己，为了修正nobj
        return chunk_alloc(size, nobj);
      }
    }

  public:
    // 分配大小为 n 的空间， n > 0
    static inline void *allocate(size_t n)
    {
      // NOTE: free_list自由链表里面每个节点存放的是实际可用内存块的首地址！
      FreeList *volatile *my_free_list;
      FreeList *result;
      // 内存请求超过阈值-->转用一级配置器
      if (n > static_cast<size_t>(__MAX_BYTES))
      {
        // return std::malloc(n);
        return _malloc_alloc_template::allocate(n); // 一级配置器
      }
      my_free_list = free_list + FREELIST_INDEX(n);
      result = *my_free_list;
      if (result == nullptr)
      {
        void *r = refill(ROUND_UP(n));
        return r;
      }
      *my_free_list = result->next;
      return result;
    }
    // 释放 _p 指向的大小为 n 的空间, _p 不能为 0
    static inline void deallocate(void *_p, size_t n)
    {
      if (n > static_cast<size_t>(__MAX_BYTES))
      {
        std::free(_p);
        return;
      }
      // TAG: reinterpret_cast 可以用于转换 nullptr 到任何指针类型，这是合法的
      FreeList *q = reinterpret_cast<FreeList *>(_p);
      FreeList *volatile *my_free_list;
      my_free_list = free_list + FREELIST_INDEX(n);
      // 调整free_list，回收区块
      q->next = *my_free_list;
      *my_free_list = q;
    }
    static void *reallocate(void *_p, size_t old_size, size_t new_size)
    {
      void *result;
      size_t copy_size;

      if (old_size > (size_t)__MAX_BYTES && new_size > (size_t)__MAX_BYTES)
      {
        return (std::realloc(_p, new_size));
      }
      if (ROUND_UP(old_size) == ROUND_UP(new_size))
        return (_p);
      result = allocate(new_size);
      copy_size = new_size > old_size ? old_size : new_size;
      // TAG: memcpy用于从源内存地址复制数据到目标内存地址
      std::memcpy(result, _p, copy_size);
      deallocate(_p, old_size);
      return (result);
    }
  };
}

#endif // !ZFWSTLSTL_ALLOC_H_