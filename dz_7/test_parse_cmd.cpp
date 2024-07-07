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
  cmd::parser_cmd parser(3);
  {
    CaptureCout cap = {std::cout};
    {
      parser.new_str("cmd1"s);
      parser.new_str("cmd2"s);
      parser.new_str("cmd3"s);
    }
    buffer = cap();
  }
  REQUIRE(buffer == "bulk: cmd1 cmd2 cmd3 \n"s);
}

TEST_CASE("Test_2") {
  std::string buffer;
  cmd::parser_cmd parser(3);
  {
    CaptureCout cap = {std::cout};
    {
      parser.new_str("cmd1"s);
      parser.new_str("cmd2"s);
      parser.new_str("cmd3"s);
      parser.new_str("cmd4"s);
    }
    buffer = cap();
  }
  REQUIRE(buffer == "bulk: cmd1 cmd2 cmd3 \n"s);
}

TEST_CASE("Test_3") {
  std::string buffer;
  cmd::parser_cmd parser(3);
  {
    {
      CaptureCout cap = {std::cout};
      parser.new_str("cmd1"s);
      parser.new_str("cmd2"s);
      parser.new_str("{"s);
      buffer = cap();
      REQUIRE(buffer == "bulk: cmd1 cmd2 \n"s);
    }
    {
      CaptureCout cap = {std::cout};
      parser.new_str("cmd3"s);
      parser.new_str("cmd4"s);
      parser.new_str("}"s);
      buffer = cap();
    }
  }
  REQUIRE(buffer == "bulk: cmd3 cmd4 \n"s);
}

TEST_CASE("Test_4") {
  std::string buffer;
  cmd::parser_cmd parser(3);
  {
    {
      CaptureCout cap = {std::cout};
      parser.new_str("cmd1"s);
      parser.new_str("cmd2"s);
      parser.new_str("{"s);
      buffer = cap();
      REQUIRE(buffer == "bulk: cmd1 cmd2 \n"s);
    }
    {
      CaptureCout cap = {std::cout};
      parser.new_str("cmd3"s);
      parser.new_str("cmd4"s);
      parser.new_str("}"s);
      buffer = cap();
      REQUIRE(buffer == "bulk: cmd3 cmd4 \n"s);
    }
    {
      CaptureCout cap = {std::cout};
      parser.new_str("{"s);
      parser.new_str("cmd5"s);
      parser.new_str("cmd6"s);
      parser.new_str("{"s);
      parser.new_str("cmd7"s);
      parser.new_str("cmd8"s);
      parser.new_str("}"s);
      parser.new_str("cmd9"s);
      parser.new_str("}"s);
      buffer = cap();
      REQUIRE(buffer == "bulk: cmd5 cmd6 cmd7 cmd8 cmd9 \n"s);
    }
    {
      CaptureCout cap = {std::cout};
      parser.new_str("{"s);
      parser.new_str("cmd10"s);
      parser.new_str("cmd11"s);
      parser.flush();
      buffer = cap();
      REQUIRE(buffer == "bulk: cmd10 cmd11 \n"s);
    }
  }
}