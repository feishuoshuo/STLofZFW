#include <iostream>
#include <string>

inline void print_start(std::string container) {
  std::cout
      << "[===============================================================]\n";
  std::cout << "[----------------- Run container test : " << container
            << " "
               "-----------------]\n";
  std::cout
      << "[-------------------------- API test ---------------------------]\n";
}

inline void print_process(std::string tmp) {
  std::cout << "[---- " << tmp << " ----]\n";
}