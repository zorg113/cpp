#pragma once
#include <string>
#include <vector>
namespace db {
using tokens = std::vector<std::string>;
class parser {
public:
  tokens parse(std::string str);
  bool validate();

private:
};
} // namespace db