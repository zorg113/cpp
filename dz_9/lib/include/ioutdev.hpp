#pragma once

#include <fstream>
#include <iostream>

#include "bulk.hpp"

namespace cmd {
/**
 * @brief class outpud data
 *
 */
class ioutdev {
public:
  virtual void update(const bulk &blk) = 0;
};
/**
 * @brief  class output data to console
 *
 */
class console : public ioutdev {
public:
  void update(const bulk &blk) final {
    std::cout << "bulk: " << blk.output() << std::endl;
  }
};
/**
 * @brief class output data to file
 *
 */
class file : public ioutdev {
public:
  void update(const bulk &blk) final {
    std::ofstream f("bulk" + blk.name()); // add thread id
    f << blk.output() << std::endl;
    f.close();
  }
};

} // namespace cmd