#include "parser.hpp"
#include <boost/tokenizer.hpp>
namespace db {
using namespace std::literals;
tokens parser::parse(std::string input) {
  using namespace boost;
  boost::char_separator<char> sep(" \t\n");
  tokenizer<boost::char_separator<char>> tok(input, sep);
  tokens ret;
  for (auto beg = tok.begin(); beg != tok.end(); ++beg) {
    ret.push_back(*beg);
  }
  return ret;
}

} // namespace db
