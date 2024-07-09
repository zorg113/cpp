#include "lib.h"

#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "Version: " << version() << std::endl;
  run_bayan(argc, argv);
  return 0;
}
