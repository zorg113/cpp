#include "lib.h"

#include "async.h"
#include "version.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

/**
 * @brief  вывод версии
 *
 * @return int номер версии
 */
int version() { return PROJECT_VERSION_PATCH; }

/**
 * @brief command handler
 *
 * @param num_cmd - size command buffer
 */
void parser_bulk(std::size_t num_cmd) {
  using namespace std::string_literals;
  auto first = []() {
    auto ctx1 = async::connect(3ul);
    std::string pack1[] = {"cmd1"s, "cmd2"s, "cmd3"s};
    async::recive(ctx1, pack1[0].c_str(), pack1[0].size());
    async::recive(ctx1, pack1[1].c_str(), pack1[1].size());
    async::recive(ctx1, pack1[2].c_str(), pack1[2].size());
    async::disconnect(ctx1);
  };
  ////////////////////////////////////////////////////////////////
  auto second = []() {
    auto ctx2 = async::connect(2ul);
    std::string pack2[] = {"cmd11"s, "{"s, "cmd12"s, "cmd13"s, "}"s, "cmd14"s};
    async::recive(ctx2, pack2[0].c_str(), pack2[0].size());
    async::recive(ctx2, pack2[1].c_str(), pack2[1].size());
    async::recive(ctx2, pack2[2].c_str(), pack2[2].size());
    async::recive(ctx2, pack2[3].c_str(), pack2[3].size());
    async::recive(ctx2, pack2[4].c_str(), pack2[4].size());
    async::recive(ctx2, pack2[5].c_str(), pack2[5].size());
    async::disconnect(ctx2);
  };
  ///////////////////////////////////////////////////////////////
  auto third = []() {
    auto ctx3 = async::connect(5ul);
    std::string pack3[] = {"cm31"s,  "cmd32"s, "cmd33"s,
                           "cmd34"s, "cmd35"s, "cmd36"s};
    async::recive(ctx3, pack3[0].c_str(), pack3[0].size());
    async::recive(ctx3, pack3[1].c_str(), pack3[1].size());
    async::recive(ctx3, pack3[2].c_str(), pack3[2].size());
    async::recive(ctx3, pack3[3].c_str(), pack3[3].size());
    async::recive(ctx3, pack3[4].c_str(), pack3[4].size());
    async::recive(ctx3, pack3[5].c_str(), pack3[5].size());
    async::disconnect(ctx3);
  };
  std::thread first_th(first);
  std::thread third_th(third);
  std::thread second_th(second);

  first_th.join();
  second_th.join();
  third_th.join();
}
