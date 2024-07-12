#include "lib.h"

#include <iostream>

int main(int argc, char ** /*argv[]*/) {

  std::cout << "Version: " << version() << std::endl;
  if (argc != 2) {
    std::cout << "Usage" << std::endl;
    std::cout << " num bulk" << std::endl;
  }
  try {
    // const int num_bulk{std::atoi(argv[1])};
    // parser_bulk(num_bulk);

  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
