#include <iostream>
#include <cstddef>
#include "../include/your_header.h"
#include "../STL/calcCMake.cpp"
// linux 命令：g++ zfw.cpp -o main |   g++ -DDEBUG zfw.cpp -o main2
int main()
{
#ifdef DEBUG
  std::cout << "Debug mode is on." << std::endl;
  int a = 0;
  int b = 2;
  for (std::size_t i = 0; i < 10; ++i)
  {
    ++b;
  }

  std::cout << a + b << std::endl;
#else
  std::cout << "Debug mode is off." << std::endl;
  YourClass yourObject;
  yourObject.yourMethod();
  int result = add(5, 3);
  std::cout << "result:" << result << std::endl;
#endif
  return 0;
}