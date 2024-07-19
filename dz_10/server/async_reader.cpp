/**
 * @brief
 *
 */

#include <boost/tokenizer.hpp>

#include "async_reader.hpp"

namespace aserver {
tokens async_reader::get_data(str &s) {
  using namespace boost;
  tokenizer<> tok(s);
  tokens ret;
  for (auto beg = tok.begin(); beg != tok.end(); ++beg) {
    ret.push_back(*beg);
  }
  return ret;
}
} // namespace aserver