#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <sstream>
#include <type_traits>
#include <typeinfo>

/**
 * @brief аллокатор для выделения памяти ограниченного размера
 *
 */
namespace best {
using namespace std::literals;

/// @brief исключение генерируемое аллокатором при исчерпании памяти
class bestalloc_exception : public std::bad_alloc {
public:
  bestalloc_exception(const std::string_view &s) : bad_alloc(), msg(s) {}
  const char *what() const throw() override { return msg.data(); }

private:
  std::string_view msg;
};

/**
 * @brief функция проверки выравнивания памяти возвращает смещение
 *
 * @param ptr указатель на память
 * @param alignment размер объекта
 * @return std::size_t
 */
inline std::size_t align_forward(const void *const ptr,
                                 const std::size_t &alignment) noexcept {
  const auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
  const auto aligned = (iptr - 1u + alignment) & -alignment;
  return aligned - iptr;
}
/**
 * @brief функция сложения адресов
 *
 * @param p указатель на память
 * @param n смещение
 * @return void* результурующий адрес
 */
inline void *ptr_add(const void *const p, const std::uintptr_t &n) noexcept {
  return reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(p) + n);
}

/// @brief аллокатор памяти совместимый со стандартной библиотекой
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
      : m_curent(m_pool), m_prev(m_pool), m_free_block(0), m_used_bytes(0) {}

  virtual ~bestalloc() noexcept {}

  template <typename U> bestalloc(const bestalloc<U, MaxSize> &a) noexcept {
    std::memmove(m_pool, a.m_pool, max_size);
    m_curent =
        reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(m_pool) +
                                 reinterpret_cast<std::uintptr_t>(a.m_curent) -
                                 reinterpret_cast<std::uintptr_t>(a.m_pool));
    m_prev =
        reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(m_pool) +
                                 reinterpret_cast<std::uintptr_t>(a.m_prev) -
                                 reinterpret_cast<std::uintptr_t>(a.m_pool));
    m_used_bytes = a.m_used_bytes;
    m_free_block = a.m_free_block;
  }

  template <class Up, class... Args> void construct(Up *p, Args &&...args) {
    ::new ((void *)p) Up(std::forward<Args>(args)...);
  }

  void destroy(pointer p) { p->~T(); }

  bestalloc select_on_container_copy_construction() const {
    return bestalloc();
  }

  /**
   * @brief метод выделения памяти
   *
   * @param n  размер в количествах объектов
   * @return T*
   */
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
  /**
   * @brief метод оствобождающий память не освобождает память
   *        а сдвигает позицию текущего адреса на предыдущую
   *        этим повышается эффективность при работе с вектором
   * @param p указатель
   * @param n размер
   */
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