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

using qeue_tf = async::queue_thread_safe<cmd::bulk>;
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
    if (m_thread.joinable())
      m_thread.join();
    m_execute.store(false);
  }
  void update() final {
    auto id = std::this_thread::get_id();
    id = id;
    while (m_execute) {
      auto blk = m_qeue.front();
      if (blk.is_stopped())
        break;
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
    // ++num;
    m_thread = std::thread(&file::update, this);
  }
  void join() {
    if (m_thread.joinable())
      m_thread.join();
    m_execute.store(false);
  }
  ~file() {
    //--num;
    join();
  }

  void update() final {
    auto id = std::this_thread::get_id();
    id = id;
    while (m_execute) {
      auto blk = m_qeue.front();
      if (blk.is_stopped())
        break;
      output(blk);
    }
    m_qeue.wake_next();
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
    std::ofstream f(ss.str());
    f << blk.output() << std::endl;
    f.close();
  }
  qeue_tf &m_qeue;
  std::atomic<bool> m_execute = true;
  std::thread m_thread;
  std::mutex m_thmutex;
  std::condition_variable m_cond;
  // static int num;
};

} // namespace async