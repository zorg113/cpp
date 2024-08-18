#pragma once
/**
 * @brief Асинхронная очередь данных
 *
 */
#include "config.hpp"
#include "serialize.hpp"
#include <atomic>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace ex_queue {

template <typename T> struct cell_t {
  std::atomic<std::uint32_t> sequence;
  T data;
  template <typename Archive> void serialize(Archive &ar, unsigned) {
    ar & sequence;
    ar & data;
  }
  cell_t() : sequence(0), data() {}
  cell_t(cell_t &&in) {
    sequence.store(in.sequence.load());
    data = in.data;
  }
  bool operator==(const cell_t &rhs) const {
    std::uint32_t val = sequence.load();
    if (val != rhs.sequence.load())
      return false;
    if (data != rhs.data)
      return false;
    return true;
  }
};

template <typename T> class equeue {
  friend class boost::serialization::access;
  queue_conf m_cfg;
  alignas(64) std::vector<cell_t<T>> m_buffer;
  alignas(64) std::uint32_t m_buffer_mask;
  alignas(64) std::atomic<std::uint32_t> m_enqueue_pos, m_dequeue_pos;
  template <class Archive> void serialize(Archive &ar, const unsigned int) {
    ar & m_cfg;
    ar & m_buffer;
    ar & m_buffer_mask;
    ar & m_enqueue_pos;
    ar & m_dequeue_pos;
  }

public:
  /// @brief конструктор с учетом конфигурирвания
  /// @param conf конфигурационная структура
  equeue(config conf)
      : m_cfg(conf), m_buffer(conf.buf_size), m_buffer_mask(conf.buf_size - 1) {
    if (m_cfg.buf_size > (1 << 30))
      throw std::runtime_error("buffer size too large");
    if (m_cfg.buf_size < 2)
      throw std::runtime_error("buffer size too small");
    if ((m_cfg.buf_size & (m_cfg.buf_size - 1)) != 0)
      throw std::runtime_error("buffer size is not power 2");
    for (int i = 0; i != m_cfg.buf_size; ++i) {
      m_buffer[i].sequence.store(i, std::memory_order_relaxed);
    }
    m_enqueue_pos.store(0, std::memory_order_relaxed);
    m_dequeue_pos.store(0, std::memory_order_relaxed);
  }
  /// @brief  конструктор по умолчанию
  equeue() {
    m_buffer.resize(m_cfg.buf_size);
    m_buffer_mask = m_cfg.buf_size - 1;
    if (m_cfg.buf_size > (1 << 30))
      throw std::runtime_error("buffer size too large");
    if (m_cfg.buf_size < 2)
      throw std::runtime_error("buffer size too small");
    if ((m_cfg.buf_size & (m_cfg.buf_size - 1)) != 0)
      throw std::runtime_error("buffer size is not power 2");
    for (int i = 0; i != m_cfg.buf_size; ++i) {
      m_buffer[i].sequence.store(i, std::memory_order_relaxed);
    }
    m_enqueue_pos.store(0, std::memory_order_relaxed);
    m_dequeue_pos.store(0, std::memory_order_relaxed);
  }

  /// @brief  добавление в lock-free очередь данных
  /// @param data  данные
  /// @return данные добавлены в очередь (true/false)
  bool push(T data) {
    cell_t<T> *cell = nullptr;
    std::uint32_t pos = 0;
    bool res = false;
    while (!res) {
      pos = m_enqueue_pos.load(std::memory_order_relaxed);
      cell = &m_buffer[pos & m_buffer_mask];
      auto seq = cell->sequence.load(std::memory_order_acquire);
      auto diff = static_cast<int>(seq) - static_cast<int>(pos);
      if (diff < 0)
        return false;
      if (diff == 0)
        res = m_enqueue_pos.compare_exchange_weak(pos, pos + 1,
                                                  std::memory_order_relaxed);
    }
    cell->data = std::move(data);
    cell->sequence.store(pos + 1, std::memory_order_release);
    return true;
  }

  /// @brief извлечение данных из очереди
  /// @param data  выходные данные
  /// @return данные извлечены из очереди (true/false)
  bool pop(T &data) {
    cell_t<T> *cell = nullptr;
    std::uint32_t pos = 0;
    bool res = false;
    while (!res) {
      pos = m_dequeue_pos.load(std::memory_order_relaxed);
      cell = &m_buffer[pos & m_buffer_mask];
      auto seq = cell->sequence.load(std::memory_order_acquire);
      auto diff = static_cast<int>(seq) - static_cast<int>(pos + 1);
      if (diff < 0)
        return false;
      if (diff == 0)
        res = m_dequeue_pos.compare_exchange_weak(pos, pos + 1,
                                                  std::memory_order_relaxed);
    }
    data = std::move(cell->data);
    cell->sequence.store(pos + m_buffer_mask + 1, std::memory_order_release);
    return true;
  }

  /// @brief проверка пустая ли очередь
  /// @return
  bool is_empty() const { return m_enqueue_pos.load() == m_dequeue_pos.load(); }

  /// @brief  размер очереди
  /// @return
  std::size_t cpacity() { return m_buffer.size(); }

  bool operator==(const equeue &rhs) const {
    if (m_cfg != rhs.m_cfg)
      return false;
    if (m_buffer != rhs.m_buffer)
      return false;
    std::uint32_t eq, dq;
    eq = m_enqueue_pos.load();
    dq = m_dequeue_pos.load();
    if (eq != rhs.m_enqueue_pos.load() || dq != rhs.m_dequeue_pos.load())
      return false;
    return true;
  }
};

} // namespace ex_queue