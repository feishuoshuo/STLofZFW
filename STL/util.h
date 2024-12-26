#ifndef ZFWSTL_UTIL_H_
#define ZFWSTL_UTIL_H_
/**
 * 包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等
 */
#include <cstddef>       //for size_t, ptrdiff_t
#include "type_traits.h" //for remove_reference
namespace zfwstl
{
  // ====================================move====================================

  template <class T>
  typename std::remove_reference<T>::type &&move(T &&arg) noexcept
  {
    return static_cast<typename std::remove_reference<T>::type &&>(arg);
  }

}
#endif // !ZFWSTL_UTIL_H_