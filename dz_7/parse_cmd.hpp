#pragma once

#include <cstddef>
#include <memory>

#include "bulk.hpp"
#include "ioutdev.hpp"

namespace cmd {

class parser_cmd {
public:
  using out = std::vector<std::unique_ptr<ioutdev>>;
  parser_cmd(size_t len) : bulk_limit(len), curr_bulk(len) {
    outputs.push_back(std::make_unique<console>()),
        outputs.push_back(std::make_unique<file>());
  }
  void new_str(const str &input);
  void flush();

private:
  size_t nest_count = 0;
  size_t bulk_limit;
  bulk curr_bulk;
  out outputs;
};

void parser_cmd::new_str(const str &input) {
  if (input != "{" && input != "}") {
    curr_bulk.push(input);
    if (curr_bulk.size() == bulk_limit && nest_count == 0) {
      flush();
    }
  } else if (input == "{") {
    if (nest_count == 0 ) {
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
void parser_cmd::flush() {
  if (curr_bulk.size() != 0) {
    for (const auto &el : outputs) {
      el->update(curr_bulk);
    }
    curr_bulk.clear();
  }
}
} // namespace cmd