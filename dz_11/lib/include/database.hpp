#pragma once

#include <unordered_map>
#include <vector>

#include "table.hpp"

int version();

namespace db {
using tokens = std::vector<std::string>;
using storage = std::unordered_map<std::string, table>;
using opt = std::optional<std::string_view>;
using ret = std::pair<bool, std::string>;

class database {
public:
  opt insert(std::string &name_table, std::size_t id, std::string &name);
  opt truncate(std::string &name_table);
  ret intersection(void);
  ret symmetric_difference(void);
  ret exec_command(tokens tkns);

private:
  storage m_storage;
};
} // namespace db