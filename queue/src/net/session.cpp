#include "session.hpp"
#include <string>

namespace aserver {

namespace sys = boost::system;
void session::do_read() {

  m_socket.async_read_some(
      net::buffer(m_data, max_length),
      [this, self = shared_from_this()](sys::error_code ec, std::size_t len) {
        if (!ec) {
          auto in = std::string(m_data, len);
          if (m_prod) {
            while (!m_queue.push(in)) {
              std::this_thread::yield();
            }
          } else {
            std::string s;
            if (!m_queue.is_empty()) {
              bool pass = m_queue.pop(s);
              if (pass) {
                constexpr std::size_t max_length_ = 1024;
                char m_out1[max_length_];
                std::strncpy(m_out1, s.c_str(), max_length_);
                m_socket.async_write_some(
                    net::buffer(m_out1, s.size() > max_length_ ? max_length_
                                                               : s.size()),
                    [](sys::error_code, std::size_t) {});
              }
            }
          }
          do_read();
        }
      });
}
} // namespace aserver