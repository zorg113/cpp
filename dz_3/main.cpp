#include "lib.h"

#include <iostream>

int main(int, char **) {
  std::cout << "Version: " << version() << std::endl;
  try {
    custom_allocator();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
