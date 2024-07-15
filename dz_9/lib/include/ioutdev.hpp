#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>

#include "bulk.hpp"
#include "qeue.hpp"
namespace async {

using qeue_tf = async::deque_thread_safe<cmd::bulk>;
/**
 * @brief class outpud data
 *
 */
class ioutdev {
public:
  virtual void update() = 0;
};

/**
 * @brief  class output data to console
 *
 */
class log : public ioutdev {
public:
  explicit log(qeue_tf &qeue) : m_qeue(qeue) {
    m_thread = std::thread(&log::update, this);
  }
  ~log() { join(); }
  void join() {
    m_execute.store(false);
    {
      std::unique_lock<std::mutex> lock(m_thmutex);
      m_cond.notify_one();
    }
    if (m_thread.joinable())
      m_thread.join();
  }
  void update() final {
    while (m_execute || !m_qeue.empty()) {
      if (m_qeue.empty()) {
        std::unique_lock<std::mutex> lock(m_thmutex);
        m_cond.wait(lock);
      }
      if (m_qeue.empty())
        continue;
      auto blk = m_qeue.front();
      output(blk);
    }
  }

private:
  void output(const cmd::bulk &blk) {
    std::cout << "bulk: " << blk.output() << std::endl;
  }
  qeue_tf &m_qeue;
  std::atomic<bool> m_execute = true;
  std::thread m_thread;
  std::mutex m_thmutex;
  std::condition_variable m_cond;
};
/**
 * @brief class output data to file
 *
 */
class file : public ioutdev {
public:
  explicit file(qeue_tf &qeue) : m_qeue(qeue) {
    m_thread = std::thread(&file::update, this);
  }
  void join() {
    m_execute.store(false);
    {
      std::unique_lock<std::mutex> lock(m_thmutex);
      m_cond.notify_one();
    }
    if (m_thread.joinable())
      m_thread.join();
  }
  ~file() { join(); }

  void update() final {
    while (m_execute || !m_qeue.empty()) {
      if (m_qeue.empty()) {
        std::unique_lock<std::mutex> lock(m_thmutex);
        m_cond.wait_for(lock, std::chrono::seconds(1));
      }
      if (m_qeue.empty())
        continue;
      auto blk = m_qeue.front();
      output(blk);
    }
  }

private:
  void output(const cmd::bulk &blk) {
    using namespace std::chrono_literals;
    std::ostringstream ss;
    std::random_device r;
    std::seed_seq seed = {r()};
    std::mt19937 e2(seed);
    auto id = std::this_thread::get_id();
    ss << "bulk" << blk.name() << " " << e2() << "_" << id;
    std::ofstream f(ss.str()); // add thread id
    f << blk.output() << std::endl;
    f.close();
  }
  qeue_tf &m_qeue;
  std::atomic<bool> m_execute = true;
  std::thread m_thread;
  std::mutex m_thmutex;
  std::condition_variable m_cond;
};

} // namespace async