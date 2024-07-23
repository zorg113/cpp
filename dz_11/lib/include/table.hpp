#pragma once
#include <map>
#include <optional>
#include <string>
namespace db {
using data = std::map<std::size_t, std::string>;
class table {
public:
  table(std::size_t init_key) : m_key(init_key) {}
  table() {};

  std::optional<std::string_view> add_row(std::size_t key, std::string &value) {
    if (m_table.find(key) != m_table.end()) {
      return {"key already exist"};
    }
    m_table[key] = value;
    return {};
  }

  void clear() { m_table.clear(); }

  auto find(std::size_t k) { return m_table.find(k); }
  auto end() { return m_table.end(); }
  auto begin() { return m_table.begin(); }

private:
  std::size_t m_key = 0;
  data m_table;
};
} // namespace db