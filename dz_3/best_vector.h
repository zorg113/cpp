#pragma once
#include "best_iterator.h"
#include <memory>
#include <utility>
/**
 * @brief чудесный вектор
 *
 */
namespace best {
template <typename T, class Allocator = std::allocator<T>> class vector {
public:
  using value_type = T;
  using allocator_type = Allocator;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using iterator = best::Iterator<value_type>;
  using const_iterator = best::Iterator<const value_type>;

  constexpr vector() noexcept(noexcept(Allocator()))
      : m_allocator(Allocator()) {}

  constexpr vector(const vector &x);
  constexpr vector(vector &&) noexcept;
  virtual ~vector();
  constexpr vector &operator=(const vector &x);

  constexpr void assign(size_type n, const_reference value);
  iterator begin() noexcept;
  iterator end() noexcept;

  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  constexpr size_type size() const noexcept;
  constexpr size_type max_size() const noexcept;
  constexpr size_type capacity() const noexcept;
  constexpr bool empty() const noexcept;
  constexpr reference operator[](size_type n);
  constexpr const_reference operator[](size_type n) const;
  constexpr const_reference at(size_type n) const;
  constexpr reference at(size_type n);
  constexpr pointer data() noexcept;
  constexpr const_pointer data() const noexcept;
  constexpr void push_back(const_reference x);
  constexpr void pop_back();
  constexpr void clear() noexcept;
  constexpr void swap(vector<T, Allocator> &other);

private:
  float m_growthFactor = 1.5f;
  Allocator m_allocator;
  pointer m_begin = nullptr;
  pointer m_last = nullptr;
  pointer m_end = nullptr;

  void reallocate(size_type n);
  void initialize(iterator, iterator);
  void destroy(iterator first, iterator last);
};
//------------------------------------
template <typename T, typename A>
constexpr vector<T, A>::vector(const vector &other) {
  reallocate(other, size());
  for (size_type i = 0; i < other.size(); ++i) {
    m_allocator.construct(m_begin + i, other[i]);
  }
}

template <typename T, typename A> vector<T, A>::~vector() {
  destroy(begin(), end());
  m_allocator.deallocate(m_begin, capacity());
}

template <typename T, typename A>
constexpr vector<T, A> &vector<T, A>::operator=(const vector<T, A> &other) {
  if (this == &other)
    return *this;
  erase(begin(), end());
  if (other.size() > capacity()) {
    reallocate(other.size());
  }
  for (size_type i = 0; i < other.size(); ++i) {
    m_allocator.construct(m_begin + i, other[i]);
  }
  m_last = m_begin + other.size();
  return *this;
}

template <typename T, typename A>
typename vector<T, A>::iterator vector<T, A>::begin() noexcept {
  return iterator(m_begin);
}

template <typename T, typename A>
typename vector<T, A>::iterator vector<T, A>::end() noexcept {
  return iterator(m_last);
}

template <typename T, typename A>
typename vector<T, A>::const_iterator vector<T, A>::cbegin() const noexcept {
  return const_iterator(m_begin);
}

template <typename T, typename A>
typename vector<T, A>::const_iterator vector<T, A>::cend() const noexcept {
  return const_iterator(m_last);
}

template <typename T, typename A>
constexpr void vector<T, A>::assign(size_type n, const_reference value) {
  erase(begin(), end());
  if (n > capacity()) {
    reallocate(n);
  }
  for (size_type i = 0; i < n; ++i) {
    m_allocator.construct(m_begin + i, value);
  }
  m_last = m_begin + n;
}

template <typename T, typename A>
constexpr void vector<T, A>::push_back(const_reference x) {
  if (size() + 1 >= capacity()) {
    reallocate(size() + 1);
  }
  m_allocator.construct(m_last++, T(x));
}

template <typename T, typename A> constexpr void vector<T, A>::pop_back() {
  m_allocator.destroy(m_last);
  --m_last;
}

template <typename T, typename A>
constexpr void vector<T, A>::swap(vector<T, A> &other) {
  std::swap(m_begin, other.m_begin);
  std::swap(m_last, other.m_last);
  std::swap(m_end, other.m_end);
  std::swap(m_allocator, other.m_allocator);
}

template <typename T, typename A>
constexpr void vector<T, A>::clear() noexcept {
  m_allocator.deallocate(m_begin, capacity());
  m_begin = m_last = m_end = nullptr;
}

template <typename T, typename A>
constexpr typename vector<T, A>::size_type
vector<T, A>::capacity() const noexcept {
  return m_end - m_begin;
}

template <typename T, typename A>
constexpr typename vector<T, A>::size_type vector<T, A>::size() const noexcept {
  return m_last - m_begin;
}

template <typename T, typename A>
constexpr typename vector<T, A>::size_type
vector<T, A>::max_size() const noexcept {
  return static_cast<size_type>(-1 / sizeof(T));
}

template <typename T, typename A>
constexpr bool vector<T, A>::empty() const noexcept {
  return size() == 0;
}

template <typename T, typename A>
constexpr typename vector<T, A>::reference vector<T, A>::at(size_type n) {
  if (n >= size()) {
    throw std::out_of_range("best::vector: out of range");
  }
  return m_begin[n];
}

template <typename T, typename A>
constexpr typename vector<T, A>::reference
vector<T, A>::operator[](size_type i) {
  return m_begin[i];
}
template <typename T, typename A>
constexpr typename vector<T, A>::pointer vector<T, A>::data() noexcept {
  return m_begin;
}
template <typename T, typename A>
constexpr typename vector<T, A>::const_pointer
vector<T, A>::data() const noexcept {
  return m_begin;
}

template <typename T, typename A>
void vector<T, A>::reallocate(size_type newCapacity) {
  if (newCapacity >= capacity()) {
    newCapacity *= m_growthFactor;
  }
  pointer newBegin = m_allocator.allocate(newCapacity);
  if (newBegin == m_begin)
    return;
  if (m_begin) {
    try {
      std::move(m_begin, m_begin + std::min(size(), newCapacity), newBegin);
    } catch (...) {
      m_allocator.deallocate(newBegin, newCapacity);
      throw;
    }
    m_allocator.deallocate(m_begin, capacity());
  }
  m_last = newBegin + size();
  m_begin = newBegin;
  m_end = newBegin + newCapacity;
}

template <typename T, typename A>
void vector<T, A>::initialize(iterator first, iterator last) {
  for (auto it = first; it != last; ++it) {
    m_allocator.construct(&*it, value_type());
  }
}

template <typename T, typename A>
void vector<T, A>::destroy(iterator first, iterator last) {
  for (auto it = first; it != last; ++it) {
    m_allocator.destroy(&*it);
  }
}
}; // namespace best