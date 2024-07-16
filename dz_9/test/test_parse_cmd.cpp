#include <catch2/catch_test_macros.hpp>

#include "lib.h"
#include "parse_cmd.hpp"
#include <iostream>
#include <sstream>
#include <string>
using namespace std::string_literals;

class CaptureCout {
public:
  CaptureCout(std::ostream &out) : out{out}, old{out.rdbuf()} {
    out.rdbuf(oss.rdbuf());
  }
  ~CaptureCout() { out.rdbuf(old); }
  std::string operator()(void) { return oss.str(); }
  void clear(void) { out.clear(), oss.clear(); }

private:
  std::ostringstream oss;
  std::ostream &out;
  std::streambuf *old;
}; // namespace Catchclass CaptureCout

TEST_CASE("Test_1") {
  std::string buffer;
  async::qeue_tf qeue_f;
  async::qeue_tf qeue_log;
  cmd::parser_cmd parser(3, qeue_f, qeue_log);
  {
    CaptureCout cap = {std::cout};
    {
      parser.new_str("cmd1"s);
      parser.new_str("cmd2"s);
      parser.new_str("cmd3"s);
    }
    buffer = cap();
  }
  REQUIRE(buffer == ""s);
}
