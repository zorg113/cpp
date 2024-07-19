#pragma once
/**
 * @brief
 *
 */

#include <string>
#include <vector>

namespace aserver {
using str = std::string;
using tokens = std::vector<str>;
class async_reader {
public:
  tokens get_data(str &str);
};
} // namespace aserver