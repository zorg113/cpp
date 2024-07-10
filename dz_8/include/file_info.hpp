#pragma once

#include <boost/filesystem.hpp>
#include <cstddef>

namespace ba {
using str = std::string;
using vec_str = std::vector<str>;
using size_t = std::size_t;
namespace fs = boost::filesystem;

/// @brief  inforation about file
struct file_info {
  file_info(const fs::path &path) : m_path(path) {}
  file_info(const fs::path &path, std::size_t size)
      : m_path(path), m_size(size) {}
  /// @brief path to file
  fs::path m_path;
  /// @brief size
  size_t m_size = 0;
  /// @brief  bulk hashes by m_bulk_size
  std::vector<size_t> m_bulk_hash;
  /// @brief bulk size for hashes
  static size_t m_bulk_size;
};

using files_info = std::vector<file_info>;
} // namespace ba