#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

#include "database.hpp"
#include "parser.hpp"
/**
 * @brief session connect
 *
 */

namespace aserver {

namespace net = boost::asio;
using strand_io = net::strand<net::io_context::executor_type>;
class session : public std::enable_shared_from_this<session> {
public:
  session(net::ip::tcp::socket socket, strand_io &strand, db::database &store)
      : m_socket(std::move(socket)), m_strand(strand), m_pa(), m_store(store) {}

  void start() { do_read(); }

private:
  void do_read();
  static constexpr std::size_t max_length = 1024;
  char m_data[max_length];
  char m_out[max_length];
  net::ip::tcp::socket m_socket;
  strand_io &m_strand;
  db::parser m_pa;
  db::database &m_store;
};
} // namespace aserver