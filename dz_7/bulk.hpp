#pragma once
#include <ctime>
#include <string>
#include <vector>

namespace cmd {
using bulk_cmd = std::vector<std::string>;
using str = std::string;
/**
 * @brief class data bulk
 *
 */
class bulk {
public:
  bulk(std::size_t len) { cmds.reserve(len); }
  void push(const std::string &cmd);
  str output() const;
  str name() const { return name_time; }
  std::size_t size() const { return cmds.size(); }
  void clear() { cmds.clear(), name_time.clear(); }

private:
  bulk_cmd cmds;
  str name_time;
};
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
