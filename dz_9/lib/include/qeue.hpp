#pragma once
#include <deque>
#include <mutex>
/**
 * @brief qeue thread safe
 *
 */

namespace async {

template <typename T> class deque_thread_safe {
public:
  /// @brief check deque is empty
  /// @return
  bool empty() const {
    std::lock_guard<std ::mutex> guard(m_mutex);
    return m_deque.empty();
  }
  /// @brief push back to deque
  /// @param obj
  void push_back(const T obj) {
    std::lock_guard<std ::mutex> guard(m_mutex);
    m_deque.push_back(obj);
  }

  /// @brief  get front element
  /// @return
  T front() {
    std::lock_guard<std ::mutex> guard(m_mutex);
    auto ret = m_deque.front();
    m_deque.pop_front();
    return ret;
  }
  /// @brief  pop front element
  void pop_front() { std::lock_guard<std ::mutex> guard(m_mutex); }
  /// @brief  clear deque
  void clear() {
    std::lock_guard<std ::mutex> guard(m_mutex);
    m_deque.clear();
  }

private:
  std::deque<T> m_deque;
  mutable std::mutex m_mutex;
};

} // namespace async