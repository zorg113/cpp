#pragma once
/**
 * @brief async server
 *
 */

#include "queue.hpp"
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/program_options.hpp>

namespace aserver {
namespace net = boost::asio;
namespace sys = boost::system;
class server {
public:
  server(net::io_context &ioc, std::size_t port,
         ex_queue::equeue<std::string> &q, bool producer)
      : m_acceptor(ioc, net::ip::tcp::endpoint(net::ip::tcp::v4(), port)),
        m_queue(q), m_prod(producer) {
    do_accept();
  }
  ~server() {}

private:
  void do_accept();
  net::ip::tcp::acceptor m_acceptor;
  ex_queue::equeue<std::string> &m_queue;
  bool m_prod;
};

} // namespace aserver