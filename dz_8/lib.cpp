
#include <boost/program_options.hpp>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include "bayan.hpp"
#include "lib.h"
#include "version.h"

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
        "compare bulk size in bytes")("hash,a",
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
    auto get_value = [&vm](const char *name, auto &value) {
      if (vm.count(name)) {
        value = vm[name].as<std::remove_reference_t<decltype(value)>>();
      }
    };
    ba::configure conf;
    get_value("target", conf.m_targets);
    get_value("exclude", conf.m_excludes);
    get_value("name", conf.m_patterns);

    get_value("depth", conf.m_depth);
    get_value("min-size", conf.m_minsize);
    get_value("bulk-size", ba::file_info::m_bulk_size);

    std::string alg;
    get_value("hash", alg);

    std::unique_ptr<ba::bayan> search(ba::create_bayan(conf, alg));
    if (search != nullptr) {
      std::vector<ba::files_info> bayans;

      bayans = search->find_bayans();
      for (auto &group : bayans) {
        for (auto &f : group) {
          std::cout << ba::fs::canonical(f.m_path) << std::endl;
        }
      }
    } else {
      std::cerr << "unknown hash algorythm " << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}