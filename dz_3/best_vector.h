#pragma once
#include "best_iterator.h"
#include <iterator>
#include <memory>

namespace best {
template <typename T, class Allocator = std::allocator<T>> class vector {
public:
  using value_type = T;
  using allocator_type = Allocator;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using reference = value_type &;
  using size_type = size_t;
  using iterator = best::Iterator<T>;
  using const_iterator = best::Iterator<const T>;

  constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {}
  constexpr explicit vector(const Allocator &) noexcept;
  constexpr explicit vector(size_type n, const Allocator &= Allocator());
  constexpr vector(size_type n, const T &value, const Allocator &= Allocator());
  constexpr vector(const vector &x);
  constexpr vector(vector &&) noexcept;
  virtual ~vector();
  constexpr vector &operator=(const vector &x);
  constexpr vector &operator=(vector &&x) noexcept(
      allocator_traits<
          Allocator>::propagate_on_container_move_assignment::value ||
      allocator_traits<Allocator>::is_always_equal::value);
  constexpr void assign(size_type n, const T &value);
  constexpr size_type size();
  constexpr size_type max_size();
  constexpr bool empty();
  constexpr reference operator[](size_type n);
  constexpr const_reference operator[](size_type n) const;
  constexpr const_reference at(size_type n);
  constexpr reference at(size_type n);
  constexpr T *data() noexcept;
  constexpr const T *data() const noexcept;
  constexpr void push_back(const T &x);
  constexpr void pop_back();
  constexpr void clear() noexcept;

private:
  Allocator m_allocator;
  pointer m_begin = nullptr;
  pointer m_last = nullptr;
  pointer m_end = nullptr;

  void reallocate(size_type n);
};
template <typename T, typename A> vector<T, A>::vector(const vector<T> &other) {
  reallocate(other, size());
  for (size_type i = 0; i < other.size(); ++i) {
    m_allocator.construct(m_begin + i, other[i]);
  }
}

template <typename T, typename A> vector<T, A>::vector(size_type n) {
  reallocate(n);
  m_last = m_begin + n;
  initialize(begin(), end());
}

template <typename T, typename A> vector<T, A>::~vector() {
  destroy(begin(), end());
  m_allocator.deallocate(m_begin, capacity());
}
}; // namespace best