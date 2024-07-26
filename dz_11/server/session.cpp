#include "session.hpp"
#include <boost/asio/post.hpp>
#include <cstring>
#include <iostream>

namespace aserver {

namespace sys = boost::system;

/**
 * @brief функция чтения обработки и записи комманд
 *
 */
void session::do_read() {
  m_socket.async_read_some(
      net::buffer(m_data, max_length),
      [this, self = shared_from_this()](sys::error_code ec, std::size_t len) {
        if (!ec) {
          auto in = std::string(m_data, len);
          /// создание очереди обработчиков комманд запросов к БД
          net::post(m_strand, [this, self = shared_from_this(), in] {
            constexpr std::size_t max_length_ = 1024;
            char m_out1[max_length_];
            auto ret = m_store.exec_command(m_pa.parse(in)).second;
            std::strncpy(m_out1, ret.c_str(), max_length_);
            m_socket.async_write_some(
                net::buffer(m_out1, ret.size() > max_length_ ? max_length_
                                                             : ret.size()),
                [](sys::error_code, std::size_t) {});
          });
          do_read();
        }
      });
}
} // namespace aserver