#include "lib.h"

#include "version.h"
#include <boost/program_options.hpp>
#include <cassert>
#include <cstdlib>
#include <iostream>

namespace po = boost::program_options;

/**
 * @brief  вывод версии
 *
 * @return int номер версии
 */
int version() { return PROJECT_VERSION_PATCH; }

void run_bayan(int argc, char *argv[]) {
  using str = std::string;

  try {
    po::options_description desc("Command line options");
    desc.add_options()("help,h", "get help")(
        "target,t", po::value<std::vector<str>>(), "target dir")(
        "exclude,e", po::value<std::vector<str>>(), "exclude dir")(
        "name,n", po::value<std::vector<str>>(), "file name pattern")(
        "depth,d", po::value<std::size_t>()->default_value(0),
        "scan depth 0 - only target dir")(
        "min-size", po::value<std::size_t>()->default_value(1),
        "minimal file size for scan in bytes")(
        "bulk-size", po::value<std::size_t>()->default_value(16),
        "compare bulk size in bytes")("hash",
                                      po::value<str>()->default_value("crc"),
                                      "hash algirithm: crc, sha254");
    po::positional_options_description pos;
    pos.add("target", -1);

    po::parsed_options parsed =
        po::command_line_parser(argc, argv).options(desc).positional(pos).run();
    po::variables_map vm;
    po::store(parsed, vm);
    if (!vm.count("target") || vm.count("help")) {
      std::cout << "Usage: " << argv[0] << " target dir ...\n"
                << desc << std::endl;
      return;
    }

  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}