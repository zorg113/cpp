#include "bulk.hpp"

namespace cmd {

/**
 * @brief push data to bulk
 *
 * @param cmd - command data
 */
void bulk::push(const str &cmd) {
  if (cmds.empty()) {
    std::time_t tm_stamp = std::time(nullptr);
    name_time = std::to_string(tm_stamp);
  }
  cmds.push_back(cmd);
}
/**
 * @brief otput data to output interface
 *
 * @return str
 */
str bulk::output() const {
  str ret;
  auto it = std::begin(cmds);
  for (; it != --std::end(cmds); ++it) {
    ret += *it + "," + " ";
  }
  ret += *it;
  return ret;
}
} // namespace cmd