#include <iostream>
#include <string>

int main(int argc, char **argv) {
  double count(0.), sum(0.);
  std::string line;
  while (std::getline(std::cin, line)) {
    sum += std::stod(line);
  }
  std::cout << sum << std::endl;
  return 0;
}