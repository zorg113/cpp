#pragma once
#include <cstdlib>
#include <map>
#include <numeric>
namespace mt {
template <typename T, T default_value, size_t dim = 2> class matrix {
public:
  using store_data = std::map<size_t, matrix<T, default_value, dim - 1>>;
  using inner_iter = typename matrix<T, default_value, dim - 1>::internal_iter;
  using iter = typename store_data::iterator;
  using citer = typename store_data::const_iterator;
  auto &operator[](size_t indx) { return m_store[indx]; }
  size_t size() const {
    return std::accumulate(
        m_store.begin(), m_store.end(), 0,
        [](size_t val, auto &ref) { return val + ref.second.size(); });
  }
  class internal_iter : public std::iterator_traits<std::forward_iterator_tag> {
  public:
    internal_iter(const iter &_ptr, store_data &stor)
        : pointer(_ptr), inner_pointer(_ptr->second.begin()), r_store(stor) {
      deeper_inner();
    }
    internal_iter(const internal_iter &iptr, store_data &stor)
        : pointer(iptr->pointer), inner_pointer(iptr->second.begin()),
          r_store(stor) {
      deeper_inner();
    }
    auto operator*() const {
      return std::make_tuple(pointer->first, inner_pointer->first,
                             inner_pointer->second);
    }
    bool operator!=(internal_iter const &iptr) const {
      return pointer != iptr.pointer;
    }
    bool operator==(internal_iter const &iptr) const {
      return pointer == iptr.pointer;
    }
    internal_iter &operator++() {
      if (inner_pointer != (*pointer).second.end())
        inner_pointer++;
      deeper_inner();
      return *this;
    }

  private:
    iter pointer;
    inner_iter inner_pointer;
    store_data &r_store;
    void deeper_inner() {
      while ((inner_pointer == (*pointer).second.end() ||
              (*pointer).second.size() == 0) &&
             pointer != r_store.end()) {
        pointer++;
        if (pointer != r_store.end() && (*pointer).second.size() != 0) {
          inner_pointer = (*pointer).second.begin();
        }
      }
    }
  };
  auto begin() { return internal_iter(m_store.begin(), m_store); }
  auto end() { return internal_iter(m_store.end(), m_store); }

private:
  store_data m_store;
};

template <typename T, T default_value> class matrix<T, default_value, 1> {
public:
  using data = std::map<size_t, T>;
  using internal_iter = typename data::iterator;
  class store_proxy {
    data &r_value;
    size_t indx;

  public:
    store_proxy(data &value, size_t _indx) : r_value(value), indx(_indx) {};
    operator T() {
      return r_value.find(indx) != r_value.end() ? r_value[indx]
                                                 : default_value;
    }
    operator const T() const {
      return r_value.find(indx) != r_value.end() ? r_value[indx]
                                                 : default_value;
    }
    store_proxy &operator=(T value) {
      if (value != default_value)
        r_value[indx] = value;
      else
        r_value.erase(indx);
      return *this;
    }
  };
  auto operator[](size_t indx) { return store_proxy(m_value, indx); }
  size_t size() const { return m_value.size(); }
  bool empty() const { return size() == 0; }
  auto begin() { return m_value.begin(); }
  auto end() { return m_value.end(); }

private:
  data m_value;
};

} // namespace mt
