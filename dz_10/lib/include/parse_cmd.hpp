#pragma once

#include <cstddef>
#include <memory>

#include "bulk.hpp"
#include "ioutdev.hpp"

namespace cmd {

/**
 * @brief class parser command
 *
 */
class parser_cmd {
public:
  parser_cmd() = delete;
  parser_cmd(parser_cmd &&) = delete;
  parser_cmd(const parser_cmd &) = delete;
  parser_cmd &operator=(const parser_cmd &) = delete;

  parser_cmd(std::size_t len, async::qeue_tf &qeue_f,
             async::qeue_tf &qeue_console)
      : bulk_limit(len), curr_bulk(len), out_file(qeue_f),
        out_console(qeue_console) {}
  void new_str(const str &input);
  void finish() {
    if (nest_count == 0)
      flush();
  }
  void flush();

private:
  std::size_t nest_count = 0;
  std::size_t bulk_limit;
  bulk curr_bulk;
  async::qeue_tf &out_file;
  async::qeue_tf &out_console;
};

} // namespace cmd