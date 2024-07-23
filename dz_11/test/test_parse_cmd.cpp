#include <catch2/catch_test_macros.hpp>

#include "database.hpp"
#include "parser.hpp"

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

TEST_CASE("Test_INSERT") {
  db::parser pa;
  db::database storage;
  auto ret = storage.exec_command(pa.parse("INSERT A 0 apple\n"s));
  REQUIRE(ret.first == true);
  REQUIRE(ret.second == "OK\n"s);
  ret = storage.exec_command(pa.parse("INSERT B 0 apple\n"s));
  REQUIRE(ret.first == true);
  REQUIRE(ret.second == "OK\n"s);
  ret = storage.exec_command(pa.parse("INSERT B 0 apple\n"s));
  REQUIRE(ret.first == false);
}

TEST_CASE("Test_TRUNCATE") {
  db::parser pa;
  db::database storage;
  auto ret = storage.exec_command(pa.parse("INSERT A 0 apple\n"s));
  REQUIRE(ret.first == true);
  REQUIRE(ret.second == "OK\n"s);
  ret = storage.exec_command(pa.parse("INSERT A 1 bananas\n"s));
  ret = storage.exec_command(pa.parse("INSERT A 1 lemon\n"s));
  ret = storage.exec_command(pa.parse("TRUNCATE A\n"s));
  REQUIRE(ret.first == true);
}

TEST_CASE("Test_INTERSECTION") {
  db::parser pa;
  db::database storage;
  auto ret = storage.exec_command(pa.parse("INSERT A 0 apple\n"s));
  ret = storage.exec_command(pa.parse("INSERT A 1 cat\n"s));
  ret = storage.exec_command(pa.parse("INSERT A 2 dog\n"s));
  ret = storage.exec_command(pa.parse("INSERT A 3 rat\n"s));
  //------------------------------------------------------------
  ret = storage.exec_command(pa.parse("INSERT B 2 cat\n"s));
  ret = storage.exec_command(pa.parse("INSERT B 3 dog\n"s));
  ret = storage.exec_command(pa.parse("INSERT B 4 rat\n"s));
  ret = storage.exec_command(pa.parse("INSERT B 5 bat\n"s));
  //-----------------------------------------------------------
  ret = storage.exec_command(pa.parse("INTERSECTION"s));
  REQUIRE(ret.first == true);
  REQUIRE(ret.second == "2,dog,cat\n3,rat,dog\n");
}

TEST_CASE("Test_SYMMETRIC_DIFFERENCE") {
  db::parser pa;
  db::database storage;
  auto ret = storage.exec_command(pa.parse("INSERT A 0 apple\n"s));
  ret = storage.exec_command(pa.parse("INSERT A 1 cat\n"s));
  ret = storage.exec_command(pa.parse("INSERT A 2 dog\n"s));
  ret = storage.exec_command(pa.parse("INSERT A 3 rat\n"s));
  //------------------------------------------------------------
  ret = storage.exec_command(pa.parse("INSERT B 2 cat\n"s));
  ret = storage.exec_command(pa.parse("INSERT B 3 dog\n"s));
  ret = storage.exec_command(pa.parse("INSERT B 4 rat\n"s));
  ret = storage.exec_command(pa.parse("INSERT B 5 bat\n"s));
  //-----------------------------------------------------------
  ret = storage.exec_command(pa.parse("SYMMETRIC_DIFFERENCE"s));
  REQUIRE(ret.first == true);
  REQUIRE(ret.second == "0,apple,\n1,cat,\n4,,rat\n5,,bat\n");
}