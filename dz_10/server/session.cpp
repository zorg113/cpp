#include "session.hpp"
#include "async.h"

namespace aserver {

namespace sys = boost::system;
void session::do_read() {

  m_socket.async_read_some(
      net::buffer(m_data, max_length),
      [this, self = shared_from_this()](sys::error_code ec, std::size_t len) {
        if (!ec) {
          auto in = str(m_data, len);
          tokens tkns = m_reader.get_data(in);
          async::handler ctx = async::connect(m_size_block);
          for (auto &tok : tkns) {
            async::recive(ctx, tok.c_str(), tok.size());
          }
          do_read();          
        }
      });
}
} // namespace aserver