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
  ///  чтение и обработка сообщений
  void do_read();
  /// максимальная длинна буфера
  static constexpr std::size_t max_length = 1024;
  /// входной буфер
  char m_data[max_length];
  /// сокет
  net::ip::tcp::socket m_socket;
  /// очередь запросов к БД
  strand_io &m_strand;
  /// парсер сообщения
  db::parser m_pa;
  /// база данных
  db::database &m_store;
};
} // namespace aserver