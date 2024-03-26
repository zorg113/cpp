#include <catch2/catch_test_macros.hpp>

#include "lib.h"

#include <string>
#include <vector>

using strings = std::vector<std::string>;
using string = std::string;
using ssize_type = std::string::size_type;

strings split(const string &str, char delim);
void lex_sort(std::vector<strings> &ip_pool);

SCENARIO("ip filter") {
  GIVEN("input string") {
    string input = "192.168.1.1\t 100\t60";
    WHEN("split strig") {
      auto result = split(input, '\t');
      REQUIRE(result.size() == 3);
      REQUIRE(result[0] == "192.168.1.1");
    }
  }
  GIVEN("pool ip") {
    std::vector<strings> pool_ip = {{"192", "168", "1", "1"},
                                    {"192", "168", "1", "2"}};
    std::vector<strings> ok = {{"192", "168", "1", "2"},
                               {"192", "168", "1", "1"}};
    WHEN("sort pool") {
      lex_sort(pool_ip);

      REQUIRE(pool_ip == ok);
    }
  }
}
