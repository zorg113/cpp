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

struct proxy_access {
  using type = async::qeue_tf *;
  friend type get_c(proxy_access);
  friend type get_f(proxy_access);
};

template <typename tag, typename tag::type M> struct extr_console {
  friend typename tag::type get_c(tag) { return M; }
};

template <typename tag, typename tag::type M> struct extr_file {
  friend typename tag::type get_f(tag) { return M; }
};

template struct extr_console<proxy_access, &async::context::qeue_console>;
template struct extr_file<proxy_access, &async::context::qeue_file>;

TEST_CASE("Test_1") {
  std::size_t p;
  {
    auto ctx = async::connect(4);
    {
      std::string pack1[] = {"cmd1"s, "cmd2"s, "cmd3"s};
      async::recive(ctx, pack1[0].c_str(), pack1[0].size());
      async::recive(ctx, pack1[1].c_str(), pack1[1].size());
      async::recive(ctx, pack1[2].c_str(), pack1[2].size());
    }
    p = get_c(proxy_access())->size();
    REQUIRE(p == 0);
    async::disconnect(ctx);
  }
  async::shutdown();
  REQUIRE(get_f(proxy_access())->size() != 0);
}
