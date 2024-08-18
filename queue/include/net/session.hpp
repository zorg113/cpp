#pragma once

#include "queue.hpp"
#include <boost/asio.hpp>
#include <string>
/**
 * @brief session connect
 *
 */

namespace aserver {

namespace net = boost::asio;

class session : public std::enable_shared_from_this<session> {
public:
  session(net::ip::tcp::socket socket, ex_queue::equeue<std::string> &q,
          bool prod)
      : m_socket(std::move(socket)), m_queue(q), m_prod(prod) {}

  void start() { do_read(); }

private:
  void do_read();
  static constexpr std::size_t max_length = 1024;
  char m_data[max_length];
  net::ip::tcp::socket m_socket;
  ex_queue::equeue<std::string> &m_queue;
  bool m_prod;
};
} // namespace aserver