#include <catch2/catch_test_macros.hpp>

#include "lib.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

class CaptureCout {
public:
  CaptureCout(std::ostream &out) : out{out}, old{out.rdbuf()} {
    out.rdbuf(oss.rdbuf());
  }
  ~CaptureCout() { out.rdbuf(old); }
  std::string operator()(void) { return oss.str(); }

private:
  std::ostringstream oss;
  std::ostream &out;
  std::streambuf *old;
}; // namespace Catchclass CaptureCout

std::string get_relative_test_path(std::string &in, std::string relative) {
  std::size_t first = in.find(relative);
  std::string ret;
  if (first != std::string::npos) {
    ret = in.substr(first);
  }
  std::size_t second = ret.rfind(relative);
  std::size_t delim = ret.find("\n");
  if (second != std::string::npos) {
    ret = ret.erase(delim, second - delim);
  }
  return ret;
}

namespace fs = std::filesystem;

TEST_CASE("bayan ") {
  int argc = 9;
  const char *argv[9] = {
      "bayan", "-t", "./../test_data", "-e", "./../test_data/ex", "-a", "sha1",
      "-d",    "1"};
  std::cerr << fs::current_path() << "\n";
  std::string buff;
  {
    CaptureCout cap = {std::cout};
    run_bayan(argc, const_cast<char **>(argv));
    buff = cap();
    buff = get_relative_test_path(buff, "test_data");
    REQUIRE(buff == "test_data/pups/cats.txt\"test_data/cats.txt\"\n");
  }
};
