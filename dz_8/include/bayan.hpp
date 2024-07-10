#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "file_info.hpp"
#include "hash.hpp"

namespace ba {
using namespace std::string_literals;
/// @brief  type hash
enum hash_type { _crc_, _sha1_ };
/// @brief  configure bayan class
struct configure {
  vec_str m_targets;
  vec_str m_excludes;
  vec_str m_patterns;
  size_t m_depth = 0;
  size_t m_minsize = 0;
  hash_type m_hash = hash_type::_crc_;
};
/// @brief  prederfine
class bayan;

/// @brief  factory methon create bayan object
/// @param conf configuration struct
/// @param alg string name alghoryhm
/// @return
bayan *create_bayan(configure &conf, const str &alg);

/// @brief  class serch file dublicate
class bayan {

  friend bayan *create_bayan(configure &conf, const str &alg);

public:
  bayan() = delete;
  bayan(const bayan &) = delete;
  bayan(bayan &&) = delete;
  bayan &operator=(const bayan &) = delete;
  bayan &operator=(bayan &&) = delete;

  /// @brief get configuretion
  /// @return configure
  configure get_config() const { return m_conf; }
  /// @brief set configuration
  /// @param conf
  void set_config(const configure &conf);
  /// @brief  find dublicate files by cintents
  /// @return vector files informations
  std::vector<files_info> find_bayans();
  /// @brief  reset list files
  void reset() { m_files.clear(); }

private:
  /// @brief  construct bayan
  /// @param config configuration checked create_bayan
  bayan(const configure &config);

  /// @brief  add files by size
  /// @param file  path to file
  /// @param min_size min size
  /// @param files files map
  void add_by_size(fs::path file, size_t min_size,
                   std::unordered_map<str, size_t> &files) const;

  /// @brief  scan direcory fo search dublicates
  /// @param targets tartget directory
  /// @param excludes exclude direcory list
  /// @param patterns patter for search
  /// @param min_size size chunk fo hash
  /// @param depth  depth recursive serch by directory
  /// @return  files array dublicate
  files_info scan_dirs(const vec_str &targets, const vec_str &excludes,
                       const vec_str &patterns, size_t min_size,
                       size_t depth) const;
  /// @brief get dublicate by size
  /// @param files array files
  /// @return files information
  files_info get_dub_size_files(files_info &files) const;

  /// @brief get group files
  /// @param files files information
  /// @return
  std::vector<files_info> get_file_groups(files_info &files) const;

  /// @brief  comare files by conten with hash
  /// @param lfile first file
  /// @param rfile secod file
  /// @return true if equal overwise false
  bool compare(file_info &lfile, file_info &rfile) const;

  /// @brief  configurate serch
  configure m_conf;
  /// @brief hasher
  std::unique_ptr<hash> m_hash;
  /// @brief  list files
  files_info m_files;
};

} // namespace ba