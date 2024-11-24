#include "/home/zhoufeiwei/Desktop/STLofZFW/src/memory/alloc.h"
#include "/home/zhoufeiwei/Desktop/STLofZFW/src/memory/alloc.cpp"
#include <iostream>
#include <limits> // for numeric_limits

// 测试：一级配置器
void test_reallocation()
{
  const size_t old_size = 128;
  const size_t new_size = 512;
  void *ptr = zfwstl::_malloc_alloc_template::allocate(old_size);
  std::cout << "Allocated " << old_size << " bytes at " << ptr << std::endl;

  // 模拟使用内存
  std::memset(ptr, 0xAA, old_size);

  // 重新分配内存
  ptr = zfwstl::_malloc_alloc_template::reallocate(ptr, old_size, new_size);
  std::cout << "Reallocated to " << new_size << " bytes at " << ptr << std::endl;

  // 模拟使用新内存
  std::memset(static_cast<char *>(ptr) + old_size, 0xBB, new_size - old_size);

  zfwstl::_malloc_alloc_template::deallocate(ptr, new_size);
  std::cout << "Deallocated memory at " << ptr << std::endl;
}

// 测试二级配置器
void test_second_level_allocator()
{
  const size_t size = 356; // 小于 __MAX_BYTES
  void *ptr = zfwstl::__default_alloc_template::allocate(size);
  std::cout << "Allocated " << size << " bytes at " << ptr << std::endl;

  // 模拟使用内存
  std::memset(ptr, 0xFF, size);

  zfwstl::__default_alloc_template::deallocate(ptr, size);
  std::cout << "Deallocated memory at " << ptr << std::endl;
}

int main()
{
  test_reallocation();
  test_second_level_allocator();
  return 0;
}