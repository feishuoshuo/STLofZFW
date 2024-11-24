#include "alloc.h"
// 静态成员变量初始化
char *zfwstl::__default_alloc_template::start_free = nullptr;
char *zfwstl::__default_alloc_template::end_free = nullptr;
size_t zfwstl::__default_alloc_template::heap_size = 0;
zfwstl::FreeList *zfwstl::__default_alloc_template::free_list[__FREELISTNUM] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr};