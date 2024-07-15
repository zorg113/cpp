#include "parse_cmd.hpp"

namespace cmd {
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
    out_file.push_back(curr_bulk);
    out_console.push_back(curr_bulk);
    curr_bulk.clear();
  }
}
} // namespace cmd