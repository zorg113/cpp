#include <catch2/catch_test_macros.hpp>

#include "lib.h"
#include "parse_cmd.hpp"
#include <iostream>
#include <sstream>
#include <string>

// namespace Catch
// class CaptureCout {
// public:
//   CaptureCout(std::ostream &out) : out{out}, old{out.rdbuf()} {
//     out.rdbuf(oss.rdbuf());
//   }
//   ~CaptureCout() { out.rdbuf(old); }
//   std::string operator()(void) { return oss.str(); }

// private:
//   std::ostringstream oss;
//   std::ostream &out;
//   std::streambuf *old;
// };

TEST_CASE("matrix simple") {
}