#include "rapidcsv.h"
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char **argv) {
  std::string line;
  while (std::getline(std::cin, line)) {
    std::stringstream sstream(line);
    rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1));
    std::cout << doc.GetColumn<float>(9)[0] << std::endl;
  }

  return 0;
}