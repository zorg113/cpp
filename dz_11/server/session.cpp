#include "session.hpp"
#include <cstring>

namespace aserver {

namespace sys = boost::system;
void session::do_read() {
  m_socket.async_read_some(
      net::buffer(m_data, max_length),
      [this, self = shared_from_this()](sys::error_code ec, std::size_t len) {
        if (!ec) {
          auto in = std::string(m_data, len);
          auto ret = m_store.exec_command(m_pa.parse(in));
          std::strncpy(m_out, ret.second.c_str(), max_length);
          m_socket.async_write_some(
              net::buffer(m_out, ret.second.size() > max_length
                                     ? max_length
                                     : ret.second.size()),
              [](sys::error_code, std::size_t) {});
          // do_read();
        }
      });
}
} // namespace aserver