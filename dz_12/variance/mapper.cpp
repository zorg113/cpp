#include "rapidcsv.h"
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

template <typename T> void map_average(T in, T &aver, T &count) {
  aver += in;
  count += 1;
}

int main(int argc, char **argv) {
  std::string line;
  double aver = 0.0, count = 0.;
  // calculate average
  std::stringstream out2;
  while (std::getline(std::cin, line)) {
    std::stringstream sstream(line);
    rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1));
    map_average(doc.GetColumn<double>(9)[0], aver, count);
    out2 << doc.GetColumn<double>(9)[0] << std::endl;
  }
  aver /= count;
  // send reduce
  while (std::getline(out2, line)) {
    auto val = std::stof(line) - aver;
    std::cout << (val * val) / (count) << std::endl;
  }
  return 0;
}