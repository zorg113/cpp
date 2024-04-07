#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <type_traits>
#include <typeinfo>

#define USE_PRETTY_FUCTION 0

namespace best {
using namespace std::literals;

class bestalloc_exception : public std::bad_alloc {
public:
  bestalloc_exception(const std::string_view &s) : bad_alloc(), msg(s) {}
  const char *what() const throw() override { return msg.data(); }

private:
  std::string_view msg;
};

inline std::size_t align_forward(const void *const ptr,
                                 const std::size_t &alignment) noexcept {
  const auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
  const auto aligned = (iptr - 1u + alignment) & -alignment;
  return aligned - iptr;
}

inline void *ptr_add(const void *const p, const std::uintptr_t &n) noexcept {
  return reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(p) + n);
}

/// @brief custom allocator
template <class T, size_t MaxSize> struct bestalloc {
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;

  static constexpr size_t max_size = MaxSize;
  char m_pool[max_size];
  void *m_curent;
  void *m_prev;
  size_t m_free_block;
  size_t m_used_bytes;

  bestalloc() noexcept
      : m_curent(m_pool), m_prev(m_pool), m_free_block(0), m_used_bytes(0) {
#if USE_PRETTY_FUCTION
    std::cout << __PRETTY_FUNCTION__ << "\n";
#endif
  }

  template <class U> bestalloc(const bestalloc<U, MaxSize> &a) noexcept {
    std::memmove(m_pool, a.m_pool, max_size);
    m_used_bytes = a.m_used_bytes;
    m_free_block = a.m_free_block;
#if USE_PRETTY_FUCTION
    std::cout << __PRETTY_FUNCTION__ << "\n";
#endif
  }

  bestalloc select_on_container_copy_construction() const {
    return bestalloc();
  }

  T *allocate(std::size_t n) {
    auto size = sizeof(T) * n;
    auto aligned = alignof(T);
    m_prev = m_curent;
    std::size_t next_pos = align_forward(m_curent, aligned);
    if (m_used_bytes + next_pos + size > max_size) {
      throw bestalloc_exception("Error bestalloc: big size"sv);
    }
    void *align_addr = ptr_add(m_curent, next_pos);
    m_curent = ptr_add(align_addr, size);
    m_used_bytes = reinterpret_cast<std::uintptr_t>(m_curent) -
                   reinterpret_cast<std::uintptr_t>(m_pool);
#if USE_PRETTY_FUCTION
    std::cout << __PRETTY_FUNCTION__ << "size byte " << size << " byte used "
              << m_used_bytes << "\n";
#endif
    return reinterpret_cast<T *>(align_addr);
  }

  void deallocate([[maybe_unused]] T *p, [[maybe_unused]] std::size_t n) {
    m_curent = m_prev;
    m_used_bytes = reinterpret_cast<std::uintptr_t>(m_curent) -
                   reinterpret_cast<std::uintptr_t>(m_pool);
#if USE_PRETTY_FUCTION
    std::cout << __PRETTY_FUNCTION__ << "\n";
#endif
  }

  template <class U> struct rebind {
    typedef bestalloc<U, MaxSize> other;
  };

  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propaganet_on_container_swap = std::true_type;
};
}; // namespace best