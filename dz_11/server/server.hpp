#pragma once
#pragma once
/**
 * @brief async server
 *
 */

#include "database.hpp"
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/program_options.hpp>

namespace aserver {
namespace net = boost::asio;
namespace sys = boost::system;
class server {
public:
  server(net::io_context &ioc, std::size_t port, db::database &store)
      : m_acceptor(ioc, net::ip::tcp::endpoint(net::ip::tcp::v4(), port)),
        m_store(store) {
    do_accept();
  }
  ~server() {}

private:
  void do_accept();
  net::ip::tcp::acceptor m_acceptor;
  db::database &m_store;
};

} // namespace aserver