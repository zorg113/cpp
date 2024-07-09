#include "hash.hpp"
#include <boost/crc.hpp>
#include <boost/functional/hash.hpp>
#include <fstream>

namespace ba {
/**
 * @brief
 *
 * @param file
 */
void crc_hash::operator()(file_info &file) {
  std::vector<char> buf(file_info::m_bulk_size, 0);
  std::ifstream f(file.m_path.native(), std::ios::in | std::ios::binary);
  f.seekg(file.m_bulk_hash.size() * file.m_bulk_size);
  f.read(buf.data(), buf.size());

  boost::crc_32_type seed;
  seed.process_bytes(buf.data(), buf.size());
  file.m_bulk_hash.push_back(seed.checksum());
}
/**
 * @brief
 *
 * @param file
 */
void sha256_hash::operator()(file_info &file) {}
} // namespace ba
