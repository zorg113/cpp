#pragma once
#include <mutex>
#include <queue>
/**
 * @brief qeue thread safe
 *
 */

namespace async {

template <typename T> T limiter();

template <typename T> class queue_thread_safe {
public:
  /// @brief push back to deque
  /// @param obj
  void push_back(const T obj) {
    std::unique_lock<std ::mutex> lock(m_mutex);
    m_deque.push(obj);
    cons.notify_one();
  }

  /// @brief  get front element
  /// @return
  T front() {
    std::unique_lock<std ::mutex> lock(m_mutex);
    cons.wait(lock, [this] { return !m_deque.empty(); });
    auto ret = m_deque.front();
    if (!ret.is_stopped()) {
      m_deque.pop();
      if (!m_deque.empty())
         cons.notify_all();
    }
    return ret;
  }
  /// @brief  clear deque
  auto size() {
    std::unique_lock<std ::mutex> lock(m_mutex);
    return m_deque.size();
  }

  void wake_and_done() {
    std::unique_lock<std ::mutex> lock(m_mutex);
    m_deque.push(limiter<T>());
    cons.notify_all();
  }
  void wake_next() { 
    std::unique_lock<std ::mutex> lock(m_mutex);
    cons.notify_all();
   }

private:
  std::condition_variable cons;
  std::queue<T> m_deque;
  mutable std::mutex m_mutex;
};

} // namespace async