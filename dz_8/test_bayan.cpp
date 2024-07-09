#include <catch2/catch_test_macros.hpp>

#include "lib.h"
#include "matrix.hpp"
#include <iostream>
#include <sstream>
#include <string>

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

TEST_CASE("matrix simple") {
  std::string buffer;
  mt::matrix<int, -1> m;
  REQUIRE(m.size() == 0);
  auto a = m[0][0];
  REQUIRE(a == -1);
  REQUIRE(m.size() == 0);
  m[100][100] = 314;
  REQUIRE(m[100][100] == 314);
  REQUIRE(m.size() == 1);
  {
    CaptureCout cap = {std::cout};
    for (auto c : m) {
      int x, y, v;
      std::tie(x, y, v) = c;
      std::cout << x << y << v << std ::endl;
    }
    buffer = cap();
  }
  REQUIRE(buffer == std::string("100100314\n"));
};

TEST_CASE("matrix ND") {
  mt::matrix<int, -1, 3> m3;
  m3[1][5][6] = 7;
  REQUIRE(m3[1][5][6] == 7);
  REQUIRE(m3.size() == 1);
  mt::matrix<int, -1, 4> m4;
  m4[1][5][6][9] = 999;
  REQUIRE(m4[1][5][6][9] == 999);
  REQUIRE(m4.size() == 1);
  mt::matrix<int, -1, 6> m6;
  m6[1][5][6][9][7][0] = 333;
  REQUIRE(m6[1][5][6][9][7][0] == 333);
  REQUIRE(m6.size() == 1);
};
