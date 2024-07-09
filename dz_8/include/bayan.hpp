#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
namespace ba {
using str = std::string;
using vec_str = std::vector<str>;
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
  configure &get_config() const { return }

private:
  configure m_conf;
  std::unique_ptr<Hash> m_hash;
};

} // namespace ba