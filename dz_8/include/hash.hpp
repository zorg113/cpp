#pragma once

#include "file_info.hpp"

namespace ba {
class hash {
public:
  virtual void operator()(file_info &file) = 0;
};

class crc_hash : public hash {
public:
  void operator()(file_info &file) override;
};

class sha1_hash : public hash {
public:
  void operator()(file_info &file) override;
};

} // namespace ba