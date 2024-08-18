
#include "server.hpp"
#include "version.h"
#include <iostream>
#include <thread>

namespace po = boost::program_options;
namespace net = boost::asio;
namespace sys = boost::system;
using namespace std::literals;

int version() { return PROJECT_VERSION_PATCH; }

/// @brief пул потоков для обслуживания клиентов
/// @tparam Fn  функция вызова
/// @param n  количество потоков
/// @param fn функция вызова
template <typename Fn> void RunWorkers(unsigned int n, const Fn &fn) {
  auto m = std::max(1u, n);
  std::vector<std::jthread> workers;
  workers.reserve(m - 1);
  while (--m) {
    workers.emplace_back(fn);
  }
  fn();
}

int main(int argc, char *argv[]) {
  std::cout << "Version: " << version() << std::endl;
  const std::size_t num_threads = std::thread::hardware_concurrency();
  try {
    po::options_description desc("Command line options");
    desc.add_options()("help,h", "get help") /// help
        ("pport,pp", po::value<std::size_t>()->required(),
         "listen producer port")("cport,cp",
                                 po::value<std::size_t>()->required(),
                                 "listen consumer port");
    po::positional_options_description pos_desc;
    pos_desc.add("pport", 1);
    pos_desc.add("cport", 2);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(desc)
                  .positional(pos_desc)
                  .run(),
              vm);
    po::notify(vm);
    net::io_context ioc(num_threads);
    /// subscribe signals shutdown
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](const sys::error_code &ec,
                              [[maybe_unused]] int signal_number) {
      if (!ec) {
        std::cout << "Signal "sv << signal_number << " recived"sv << std::endl;
        ioc.stop();
      }
    });
    ex_queue::equeue<std::string> q;
    aserver::server producer(ioc, vm["pport"].as<std::size_t>(), std::ref(q),
                             true);
    aserver::server consumer(ioc, vm["cport"].as<std::size_t>(), std::ref(q),
                             false);
    std::cout << "server_run" << std::endl;
    RunWorkers(num_threads, [&ioc] { ioc.run(); });

  } catch (po::error &er) {
    std::cerr << er.what() << "\n";
  }
  return 0;
}