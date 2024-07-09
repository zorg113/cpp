

#include <algorithm>
#include <memory>
#include <regex>

#include "bayan.hpp"

namespace ba {

std::size_t file_info::m_bulk_size = 0;

bayan::bayan(const configure &conf) { set_config(conf); }

void bayan::set_config(const configure &conf) {
  m_conf = conf;
  if (conf.m_hash == hash_type::_crc_) {
    m_hash = std::make_unique<crc_hash>();
  }
}

std::vector<files_info> bayan::find_bayans() {
  if (m_files.empty()) {
    m_files = scan_dirs(m_conf.m_targets, m_conf.m_excludes, m_conf.m_patterns,
                        m_conf.m_minsize, m_conf.m_depth);
  }
  files_info dupl_size_files;
  while (!(dupl_size_files = get_dub_size_files(m_files)).empty()) {
    auto bayans = get_file_groups(dupl_size_files);
    if (!bayans.empty()) {
      return bayans;
    }
  }
  return {};
}

files_info bayan::scan_dirs(const vec_str &targets, const vec_str &excludes,
                            const vec_str &patterns, std::size_t min_size,
                            std::size_t depth) const {
  auto files = std::unordered_map<str, std::size_t>();
  for (auto &t : targets) {
    fs::recursive_directory_iterator it(t), end;
    for (; it != end; ++it) {
      if (fs::is_regular_file(it->status()) &&
          !fs::is_symlink(it->symlink_status())) {
        if (patterns.empty())
          add_by_size(it->path(), min_size, files);
        for (auto &pattern : patterns) {
          std::regex expr(pattern, std::regex_constants::icase);
          if (std::regex_match(it->path().filename().native(), expr))
            add_by_size(it->path(), min_size, files);
        }
      } else if (fs::is_directory(it.status())) {
        if (it.depth() >= depth) {
          it.no_push();
        }
        auto abs_path = fs::canonical(*it);
        for (auto &exclude : excludes) {
          if (abs_path == fs::canonical(exclude)) {
            it.no_push();
          }
        }
      }
    }
  }
  files_info files_inf;
  files_inf.reserve(files.size());
  std::transform(files.begin(), files.end(), std::back_inserter(files_inf),
                 [](auto &pair) { return file_info(pair.first, pair.second); });
  return files_inf;
}

void bayan::add_by_size(fs::path file, std::size_t min_size,
                        std::unordered_map<str, std::size_t> &files) const {
  auto file_size = fs::file_size(file);
  if (file_size >= min_size) {
    files.insert({file.native(), file_size});
  }
}

files_info bayan::get_dub_size_files(files_info &files) const {
  files_info dubl_files;
  if (!files.empty()) {
    for (;;) {
      auto it = std::partition(
          files.begin(), files.end(),
          [size = files[0].m_size](const auto &f) { return f.m_size != size; });
      dubl_files.insert(dubl_files.end(), std::make_move_iterator(it),
                        std::make_move_iterator(files.end()));
      files.erase(it, files.end());
      if (dubl_files.size() > 1 || dubl_files.empty())
        break;
      else
        dubl_files.clear();
    }
  }
  return dubl_files;
}

std::vector<files_info> bayan::get_file_groups(files_info &files) const {
  std::vector<files_info> dubl_files_group;
  for (std::size_t i = 0; i < files.size(); ++i) {
    auto it = std::partition(
        ++files.begin(), files.end(),
        [this, file = files[0]](auto &f) mutable { return !compare(file, f); });
    if (std::distance(it, files.end()) > 0) {
      files_info fsi;
      fsi.push_back(std::move(files[0]));
      fsi.insert(fsi.end(), std::make_move_iterator(it),
                 std::make_move_iterator(files.end()));
      dubl_files_group.push_back(std::move(fsi));
    }
    files.erase(it, files.end());
    files.erase(files.begin());
  }
  return dubl_files_group;
}

bool bayan::compare(file_info &lfi, file_info &rfi) const {
  std::size_t step = 0;
  while ((step * file_info::m_bulk_size) < lfi.m_size) {
    auto steps = std::min(lfi.m_bulk_hash.size(), rfi.m_bulk_hash.size());
    for (; step < steps; ++step) {
      if (lfi.m_bulk_hash[step] != rfi.m_bulk_hash[step])
        return false;
    }
    if (lfi.m_bulk_hash.size() < rfi.m_bulk_hash.size())
      (*m_hash)(lfi);
    else
      (*m_hash)(rfi);
  }
  return true;
}

} // namespace ba