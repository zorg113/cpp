#include "config.hpp"
#include <boost/json.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace ex_queue {
using namespace std::literals;
namespace json = boost::json;

/// @brief
/// @param path_to_file
/// @return
config load_config(const std::filesystem::path &path_to_file) {
  if (!std::filesystem::exists(path_to_file))
    throw std::runtime_error("file "s + path_to_file.c_str() + " not found"s);
  std::ifstream json_conf(path_to_file.c_str(), std::ios::in);
  std::stringstream json_buffer;
  json_buffer << json_conf.rdbuf();
  auto val = json::parse(json_buffer.str()).as_object();
  bool verbose = val["verbose"].as_bool();
  std::int32_t buf_size = val["buf_size"].as_int64();
  return {verbose,
          buf_size,
          DEF_NTASKS,
          DEF_MAX_THREADS,
          std::chrono::milliseconds(0),
          std::chrono::milliseconds(0)};
} // namespace boost::jsonl

/// @brief
/// @param cfg
/// @return
bool queue_conf::operator==(const queue_conf &cfg) const {
  if (verbose != cfg.verbose || buf_size != cfg.buf_size)
    return false;
  return true;
}
/// @brief
/// @param cfg
/// @return
bool queue_conf::operator!=(const queue_conf &cfg) const {
  return !this->operator==(cfg);
}
} // namespace ex_queue