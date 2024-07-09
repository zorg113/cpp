#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "file_info.hpp"
#include "hash.hpp"

namespace ba {

enum hash_type { _crc_, _sha256_ };
/// @brief  configure bayan class
struct configure {
  vec_str m_targets;
  vec_str m_excludes;
  vec_str m_patterns;
  size_t m_depth = 0;
  size_t m_minsize = 0;
  hash_type m_hash = hash_type::_crc_;
};
/// @brief  class serch file dublicate
class bayan {
public:
  bayan(const configure &config);
  bayan() = delete;
  bayan &operator=(const bayan &) = delete;
  configure get_config() const { return m_conf; }
  void set_config(const configure &conf);
  std::vector<files_info> find_bayans();
  void reset() { m_files.clear(); }

private:
  files_info scan_dirs(const vec_str &targets, const vec_str &excludes,
                       const vec_str &patterns, size_t min_size,
                       size_t depth) const;
  void add_by_size(fs::path file, size_t min_size,
                   std::unordered_map<str, size_t> &files) const;

  files_info get_dub_size_files(files_info &files) const;

  std::vector<files_info> get_file_groups(files_info &files) const;

  bool compare(file_info &lfile, file_info &rfile) const;

  configure m_conf;
  std::unique_ptr<hash> m_hash;
  files_info m_files;
};

} // namespace ba