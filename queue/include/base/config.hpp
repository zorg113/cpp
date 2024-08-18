#pragma once
#include <chrono>
#include <filesystem>
namespace ex_queue {
constexpr bool DEF_VERBOSE = false;
constexpr int DEF_BUFSIZE = 1024;
constexpr int DEF_NTASKS = 100000;
constexpr int DEF_MAX_THREADS = 10;
constexpr int DEF_PTIME = 0;
constexpr int DEF_CTIME = 0;

struct config {
  bool verbose = DEF_VERBOSE;
  int buf_size = DEF_BUFSIZE;
  int n_tasks = DEF_NTASKS;
  int n_threads = DEF_MAX_THREADS;
  std::chrono::milliseconds p_time;
  std::chrono::milliseconds c_time;
};

struct queue_conf {
  bool verbose = DEF_VERBOSE;
  int buf_size = DEF_BUFSIZE;
  queue_conf(const config &cfg)
      : verbose(cfg.verbose), buf_size(cfg.buf_size) {}
  queue_conf() {}
  bool operator==(const queue_conf &cfg) const;
  bool operator!=(const queue_conf &cfg) const;
};

config load_config(const std::filesystem::path &path_to_file);

} // namespace ex_queue

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive &ar, ex_queue::queue_conf &c, const unsigned int) {
  ar & c.verbose;
  ar & c.buf_size;
}

} // namespace serialization
} // namespace boost