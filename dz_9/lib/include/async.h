#pragma once
#include "parse_cmd.hpp"
#include <iostream>
#include <memory>
/**
 * @brief  interface async bulk library
 *
 */
namespace async {
/// @brief  class context information about context
class context;

using handler = std::unique_ptr<context>;

/**
 * @brief create command handler
 *
 * @param bulk_size  number command
 * @param out_stream  stream log output
 * @return context handler
 */
handler connect(std::size_t bulk_size, std::ostream &out_stream = std::cout);

/**
 * @brief get command to handler
 *
 * @param ctx handler command
 * @param data data input
 * @param size_data size_data bulk
 */
void recive(handler &ctx, const char *data, std::size_t size_data);

/**
 * @brief close handler
 *
 * @param ctx handler
 */
void disconnect(handler &ctx);

/**
 * @brief shudown thread in library
 *
 */
void shutdown(void);

class context {

public:
  context(const std::size_t len);

  void run(const char *data, const std::size_t size_data);
  ~context();
  static void shutdown();

private:
  std::unique_ptr<cmd::parser_cmd> m_parser;
  static std::mutex mutx;
  static std::atomic<int64_t> counter;
  static std::unique_ptr<log> to_log;
  static std::unique_ptr<file> to_file[2];
  static qeue_tf qeue_file;
  static qeue_tf qeue_console;
};
} // namespace async