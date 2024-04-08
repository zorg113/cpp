#include <catch2/catch_test_macros.hpp>

#include "best_allocator.h"
#include "best_vector.h"
#include "lib.h"
#include <string>
#include <vector>

SCENARIO("allocator") {
  GIVEN("a stl vector of doubles") {
    std::vector<double, best::bestalloc<double, 800>> v;
    WHEN("add 10 doubles to vector") {
      for (int i = 0; i < 10; i++) {
        v.push_back(i);
      }
      THEN("the vector size is 10") {
        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() == 16);
      }
    }
  }
  GIVEN("a best vector with best allocator") {
    best::vector<double, best::bestalloc<double, 800>> v;
    WHEN("add 10 doubles to vector") {
      for (int i = 0; i < 10; i++) {
        v.push_back(i);
      }
      THEN("the vector size is 10") {
        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() == 10);
      }
    }
    WHEN("check iterator on best vector") {
      for (int i = 0; i < 10; i++) {
        v.push_back(i);
      }
      int n = 0;
      for (auto &val : v) {
        REQUIRE(val == n);
        ++n;
      }
    }
  }
}
