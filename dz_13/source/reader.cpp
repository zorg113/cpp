#include "reader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace reader {

bool readCoefs(std::istream &stream, std::vector<float> &coefs) {
  std::string line;
  std::getline(stream, line);

  coefs.clear();
  std::istringstream linestream{line};
  double value;
  while (linestream >> value) {
    coefs.push_back(value);
  }

  return stream.good();
}

bool readFeatures(std::istream &stream, std::vector<float> &features,
                  int &targetClass) {
  std::string line;
  std::getline(stream, line);

  std::istringstream linestream{line};
  bool first = true;

  features.clear();
  for (std::string str; std::getline(linestream, str, ',');) {
    if (first) {
      first = false;
      targetClass = std::stoi(str);
    } else {
      features.push_back(std::stoi(str));
    }
  }

  return stream.good();
}

bool readFeaturesTest(std::istream &stream, std::vector<float> &features,
                      int &targetClass) {
  std::string line;
  std::getline(stream, line);  

  std::istringstream linestream{line};
  bool first = true;
  try {
    features.clear();
    for (std::string str; std::getline(linestream, str, ' ');) {
      if (first) {
        first = false;
      } else {
        features.push_back(std::stoi(str));
      }
    }
  } catch (std::invalid_argument &e) {
    std::cerr << "invalid argument " << e.what() << std::endl;
  }

  return stream.good();
}

} // namespace reader
