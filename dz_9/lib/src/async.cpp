#include "async.h"

namespace async {
/**
 * @brief Construct a new context::context object
 *
 * @param len
 */
context::context(const std::size_t len) {
  m_parser = std::make_unique<cmd::parser_cmd>(len, qeue_file, qeue_console);
}
/**
 * @brief
 *
 * @param data
 * @param size_data
 */
void context::run(const char *data, const std::size_t size_data) {
  m_parser->new_str(data);
}

/**
 * @brief Destroy the context::context object
 *
 */
context::~context() { m_parser->finish(); }

/// @brief  can be removed to the library configuration
qeue_tf context::qeue_file;
qeue_tf context::qeue_console;
log context::to_log(qeue_console);
file context::to_file[2] = {file(context::qeue_file), file(context::qeue_file)};

/**
 * @brief create command handler
 *
 * @param bulk_size  number command
 * @param out_stream  stream log output
 * @return context handler
 */
handler connect(std::size_t bulk_size, std::ostream &out_stream) {
  return std::make_unique<context>(bulk_size);
}

/**
 * @brief get command to handler
 *
 * @param ctx handler command
 * @param data data input
 * @param size_data size_data bulk
 */
void recive(handler &ctx, const char *data, std::size_t size_data) {
  ctx->run(data, size_data);
}

/**
 * @brief close handler
 *
 * @param ctx handler
 */
void disconnect(handler &) {}

} // namespace async