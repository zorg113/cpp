#include "async.h"
#include <mutex>

namespace async {

template <> cmd::bulk limiter<cmd::bulk>() { return cmd::bulk(0, -1); };

/**
 * @brief Construct a new context::context object
 *
 * @param len
 */
context::context(const std::size_t len) {
  const std::lock_guard<std::mutex> lock(mutx);
  auto a = counter.load();
  if (a == 0) {
    context::to_log = std::make_unique<log>(qeue_console);
    context::to_file[0] = std::make_unique<file>(qeue_file);
    context::to_file[1] = std::make_unique<file>(qeue_file);
    counter.store(1);
  }
  m_parser = std::make_unique<cmd::parser_cmd>(len, qeue_file, qeue_console);
}
/**
 * @brief
 *
 * @param data
 * @param size_data
 */
void context::run(const char *data, const std::size_t) {
  m_parser->new_str(data);
}

/**
 * @brief Destroy the context::context object
 *
 */
context::~context() { m_parser->finish(); }

void context::shutdown() {
  std::unique_lock<std::mutex> lock(mutx);
  qeue_file.wake_and_done();
  qeue_console.wake_and_done();
}

/// @brief  can be removed to the library configuration
qeue_tf context::qeue_file;
qeue_tf context::qeue_console;
std::mutex context::mutx;
std::unique_ptr<log> context::to_log = nullptr;
std::unique_ptr<file> context::to_file[2] = {nullptr, nullptr};
std::atomic<int64_t> context::counter = 0;
// int file::num = 0;

/**
 * @brief create command handler
 *
 * @param bulk_size  number command
 * @param out_stream  stream log output
 * @return context handler
 */
handler connect(std::size_t bulk_size, std::ostream & /*out_stream*/) {
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
void disconnect(handler &ctx) { ctx.reset(); }

void shutdown(void) { context::shutdown(); }
} // namespace async