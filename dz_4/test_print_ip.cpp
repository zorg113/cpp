#include <catch2/catch_test_macros.hpp>

#include "lib.h"
#include "parse_ip.hpp"

#include <array>
#include <sstream>
#include <string>
#include <vector>

// namespace Catch

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
};

TEST_CASE("parser ip POD") {
  std::string buffer;
  //------------------------------------------
  {
    CaptureCout cap = {std::cout};
    print_ip(int8_t{-1});
    buffer = cap();
  }
  REQUIRE(buffer == std::string("255\n"));
  //------------------------------------------
  {
    CaptureCout cap = {std::cout};
    print_ip(int16_t{0});
    buffer = cap();
  }
  REQUIRE(buffer == std::string("0.0\n"));
  //------------------------------------------
  {
    CaptureCout cap = {std::cout};
    print_ip(int32_t{2130706433});
    buffer = cap();
  }
  REQUIRE(buffer == std::string("127.0.0.1\n"));
  //------------------------------------------
  {
    CaptureCout cap = {std::cout};
    print_ip(int64_t{8875824491850138409});
    buffer = cap();
  }
  REQUIRE(buffer == std::string("123.45.67.89.101.112.131.41\n"));
};

TEST_CASE("parser ip container and string") {
  std::string buffer;
  //------------------------------------------
  {
    CaptureCout cap = {std::cout};
    print_ip(std::vector<int>{100, 200, 300, 400});
    buffer = cap();
  }
  REQUIRE(buffer == std::string("100.200.300.400\n"));
  //------------------------------------------
  {
    CaptureCout cap = {std::cout};
    print_ip(std::list<short>{400, 300, 200, 100});
    buffer = cap();
  }
  REQUIRE(buffer == std::string("400.300.200.100\n"));
  //------------------------------------------
  {
    CaptureCout cap = {std::cout};
    print_ip(std::string{"Hello, World !"});
    buffer = cap();
  }
  REQUIRE(buffer == std::string("Hello, World !\n"));
};

TEST_CASE("parser ip tuple") {
  std::string buffer;
  //-------------------------------------------------
  {
    CaptureCout cap = {std::cout};
    print_ip(std::make_tuple(123, 456, 789, 0));
    buffer = cap();
  }
  REQUIRE(buffer == std::string("123.456.789.0\n"));
}