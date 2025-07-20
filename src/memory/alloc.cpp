#include "alloc.h"

namespace zfwstl {
// 静态成员变量初始化
char *zfwstl::__default_alloc_template::start_free = nullptr;
char *zfwstl::__default_alloc_template::end_free = nullptr;
size_t zfwstl::__default_alloc_template::heap_size = 0;
zfwstl::FreeList *zfwstl::__default_alloc_template::free_list[__FREELISTNUM] = {
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

void global_oom_handler() {
  std::cerr << "Out of Memory! Taking emergency actions..." << std::endl;
  // 这里可以添加一些紧急处理措施，比如尝试释放缓存，或者减少内存使用等
  std::abort();
}

void (*_malloc_alloc_template::__malloc_alloc_oom_handler)() =
    global_oom_handler;
} // namespace zfwstl