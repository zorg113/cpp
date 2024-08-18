#include <memory>

#include "server.hpp"
#include "session.hpp"

namespace aserver {

void server::do_accept() {
  m_acceptor.async_accept([this](sys::error_code ec,
                                 net::ip::tcp::socket socket) {
    if (!ec) {
      std::make_shared<session>(std::move(socket), m_queue, m_prod)->start();
    }
    do_accept();
  });
}

} // namespace aserver