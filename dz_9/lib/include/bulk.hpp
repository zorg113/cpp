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

} // namespace cmd
