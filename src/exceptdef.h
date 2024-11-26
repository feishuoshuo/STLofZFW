#ifndef ZFWSTL_EXCEPTDEF_H_
#define ZFWSTL_EXCEPTDEF_H_
/*
宏： 如果ZFWSTL_EXCEPTDEF_H_未定义，则定义它并继续编译，
否则跳过整个头文件的内容。这样可以确保头文件只被包含一次，
避免了重定义错误。*/
#include <stdexcept> // 标准异常类 for runtime_error,length_error...
#include <cassert>   // 断言 for assert 用于调试时检查条件是否为真

namespace zfwstl
{

#define MYSTL_DEBUG(expr) \
  assert(expr) // 断言检查

#define THROW_LENGTH_ERROR_IF(expr, what) \
  if ((expr))                             \
  throw std::length_error(what)

#define THROW_OUT_OF_RANGE_IF(expr, what) \
  if ((expr))                             \
  throw std::out_of_range(what)

#define THROW_RUNTIME_ERROR_IF(expr, what) \
  if ((expr))                              \
  throw std::runtime_error(what)

} // namepsace zfwstl

#endif // !ZFWSTL_EXCEPTDEF_H_
