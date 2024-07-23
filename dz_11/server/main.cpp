

#include <iostream>

// #include "async.h"
// #include "lib.h"
#include "database.hpp"
#include "server.hpp"
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace net = boost::asio;
namespace sys = boost::system;
using namespace std::literals;

template <typename Fn> void Worker(std::size_t num_threads, const Fn &fn) {}

int main(int argc, char *argv[]) {
  std::cout << "Version: " << version() << std::endl;
  const std::size_t num_threads = std::thread::hardware_concurrency();
  db::database storage;
  try {
    po::options_description desc("Command line options");
    desc.add_options()("help,h", "get help") /// help
        ("port,p", po::value<std::size_t>()->required(),
         "listen port"); // listen port
    po::positional_options_description pos_desc;
    pos_desc.add("port", 1);
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
    aserver::server serv(ioc, vm["port"].as<std::size_t>(), storage);
    ioc.run();

  } catch (po::error &er) {
    std::cerr << er.what() << "\n";
  }
  return 0;
}