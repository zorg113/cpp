#include "lib.h"

#include <iostream>

int main(int, char **) {
  std::cout << "Version: " << version() << std::endl;
  run_mvc();
  return 0;
}
