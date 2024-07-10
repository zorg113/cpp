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
  using out = std::vector<std::unique_ptr<ioutdev>>;
  parser_cmd(std::size_t len) : bulk_limit(len), curr_bulk(len) {
    outputs.push_back(std::make_unique<console>()),
        outputs.push_back(std::make_unique<file>());
  }
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
  out outputs;
};
/**
 * @brief parse new string to command
 *
 * @param input
 */
void parser_cmd::new_str(const str &input) {
  if (input != "{" && input != "}") {
    curr_bulk.push(input);
    if (curr_bulk.size() == bulk_limit && nest_count == 0) {
      flush();
    }
  } else if (input == "{") {
    if (nest_count == 0) {
      flush();
    }
    ++nest_count;
  } else {
    --nest_count;
    if (nest_count < 1) {
      flush();
      nest_count = 0;
    }
  }
}
/**
 * @brief flush command
 *
 */
void parser_cmd::flush() {
  if (curr_bulk.size() != 0) {
    for (const auto &el : outputs) {
      el->update(curr_bulk);
    }
    curr_bulk.clear();
  }
}
} // namespace cmd