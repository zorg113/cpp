#pragma once
/**
 * @brief async server
 *
 */

#include "async.h"
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/program_options.hpp>

namespace aserver {
namespace net = boost::asio;
namespace sys = boost::system;
class server {
public:
  server(net::io_context &ioc, std::size_t port, std::size_t size_block)
      : m_acceptor(ioc, net::ip::tcp::endpoint(net::ip::tcp::v4(), port)),
        m_size_block(size_block) {
    do_accept();
  }
  ~server() { async::shutdown(); }

private:
  void do_accept();
  net::ip::tcp::acceptor m_acceptor;
  std::size_t m_size_block;
};

} // namespace aserver