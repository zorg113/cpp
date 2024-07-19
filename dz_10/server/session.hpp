#pragma once

#include <boost/asio.hpp>

#include "async_reader.hpp"
/**
 * @brief session connect
 *
 */

namespace aserver {

namespace net = boost::asio;

class session : public std::enable_shared_from_this<session> {
public:
  session(net::ip::tcp::socket socket, std::size_t size_block)
      : m_socket(std::move(socket)), m_size_block(size_block), m_reader() {}

  void start() { do_read(); }

private:
  void do_read();
  static constexpr std::size_t max_length = 1024;
  char m_data[max_length];
  net::ip::tcp::socket m_socket;
  std::size_t m_size_block;
  async_reader m_reader;
};
} // namespace aserver