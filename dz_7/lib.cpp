#include "lib.h"

#include "parse_cmd.hpp"
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

void parser_bulk(size_t num_cmd) {
  cmd::str data;
  cmd::parser_cmd parser(num_cmd);
  while (std::cin >> data) {
    parser.new_str(data);
  }
  parser.finish();
}