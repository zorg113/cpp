#include "database.hpp"
#include <iostream>
#include <sstream>
#include <version.h>

/**
 * @brief  вывод версии
 *
 * @return int номер версии
 */
int version() { return PROJECT_VERSION_PATCH; }

namespace db {
using namespace std::literals;

/**
 * @brief
 *
 * @param name_table
 * @param id
 * @param name
 * @return opt
 */
opt database::insert(std::string &name_table, std::size_t id,
                     std::string &name) {
  if (m_storage.contains(name_table)) {
    auto &table = m_storage[name_table];
    return table.add_row(id, name);
  }
  return m_storage[name_table].add_row(id, name);
}

/**
 * @brief
 *
 * @param name_table
 * @return opt
 */
opt database::truncate(std::string &name_table) {
  opt ret = {};
  if (m_storage.contains(name_table)) {
    m_storage[name_table].clear();
    return ret;
  }
  return {"ERR message: table "s + name_table + "not found"};
}
/**
 * @brief
 *
 * @return ret
 */
ret database::intersection(void) {
  std::ostringstream ss;
  if (m_storage.size() == 2) {
    if (!m_storage["A"].empty() && !m_storage["B"].empty()) {
      auto it1 = m_storage["A"];
      auto it2 = m_storage["B"];
      for (const auto &[k, v1] : it1) {
        if (auto val = it2.find(k); val != it2.end()) {
          ss << k << "," << v1 << "," << val->second << std::endl;
        }
      }
    }
    return {true, ss.str()};
  }
  return {false, ""};
}

/**
 * @brief
 *
 * @return ret
 */
ret database::symmetric_difference(void) {
  std::ostringstream ss;
  if (m_storage.size() == 2) {
    auto it1 = m_storage["A"];
    auto it2 = m_storage["B"];
    for (const auto &[k, v1] : it1) {
      if (auto val = it2.find(k); val == it2.end()) {
        ss << k << "," << v1 << "," << std::endl;
      }
    }
    for (const auto &[k, v2] : it2) {
      if (auto val = it1.find(k); val == it1.end()) {
        ss << k << ","
           << "," << v2 << std::endl;
      }
    }
    return {true, ss.str()};
  }
  return {false, ""};
}

/**
 * @brief
 *
 * @param tkns
 * @return ret
 */
ret database::exec_command(tokens tkns) {
  ret status;
  auto tok = tkns[0];
  if (tok == "INSERT") {
    auto r = insert(tkns[1], std::atoi(tkns[2].c_str()), tkns[3]);
    if (r != std::nullopt) {
      status = std::make_pair(false, r.value());
    } else
      status = std::make_pair(true, "OK\n"s);
  }
  if (tok == "TRUNCATE") {
    auto r = truncate(tkns[1]);
    if (r != std::nullopt) {
      status = std::make_pair(false, *r);
    } else
      status = std::make_pair(true, "OK\n"s);
  }
  if (tok == "INTERSECTION") {
    status = intersection();
    if (status.first) {
      status.second += "OK\n";
    } else
      status.second += "ERR message\n";
  }
  if (tok == "SYMMETRIC_DIFFERENCE") {
    status = symmetric_difference();
    if (status.first) {
      status.second += "OK\n";
    } else {
      status.second += "ERR message\n";
    }
  }
  return status;
}
} // namespace db
