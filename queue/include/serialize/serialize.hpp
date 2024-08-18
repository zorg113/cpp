#pragma once
#include <atomic>

namespace boost {
namespace serialization {

template <typename Archive>
void serialize(Archive &ar, std::atomic<uint32_t> &t, unsigned) {
  uint32_t val = t.load();
  ar & val;
  t.store(val);
}
} // namespace serialization
} // namespace boost