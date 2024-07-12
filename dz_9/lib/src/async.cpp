#include "async.h"
#include "parse_cmd.hpp"

namespace async {

class context {
public:
  context(const std::size_t len) {
    m_parser = std::make_unique<cmd::parser_cmd>(len);
  }

  void run(const char *data, const std::size_t size_data) {
    m_parser->new_str(data);
  }
  ~context() { m_parser->finish(); }

private:
  std::unique_ptr<cmd::parser_cmd> m_parser = nulptr;
  std::size_t id = 0;
};
} // namespace async