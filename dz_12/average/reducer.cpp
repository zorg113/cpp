#include <iostream>
#include <string>

int main(int argc, char **argv) {
  double count(0.), sum(0.);
  std::string line;
  while (std::getline(std::cin, line)) {
    count += 1.;
    sum += std::stod(line);
  }
  std::cout << sum / count << std::endl;
  return 0;
}