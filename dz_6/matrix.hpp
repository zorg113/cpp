#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <vector>
/**
 * @brief Вывод значения адреса побайтово через .
 *        для простых типов данных
 * @tparam T шаблонный тип
 * @param ip адресс
 * @return std::enable_if<std::is_integral<T>::value>::type
 */
template <typename T>
typename std::enable_if<std::is_integral<T>::value>::type
print_ip(const T &ip) {
  auto byte = reinterpret_cast<const uint8_t *>(&ip);
  auto pos = byte + sizeof(T) - 1;
  for (; pos != byte; --pos) {
    std::cout << (uint16_t)(*pos) << ".";
  }
  std::cout << (uint16_t)(*pos) << "\n";
}
/**
 * @brief проверка подстановки вектора
 *
 * @tparam Type
 */
template <class Type> struct is_std_vector {
  static constexpr bool value = false;
};
/**
 * @brief проверка подстановки вектора
 *
 * @tparam Type
 */
template <class Type> struct is_std_vector<std::vector<Type>> {
  static constexpr bool value = true;
};
/**
 * @brief проверка подстановки списка
 *
 * @tparam Type
 */
template <class Type> struct is_std_list {
  static constexpr bool value = false;
};
/**
 * @brief проверка подстановки списка
 *
 * @tparam Type
 */
template <class Type> struct is_std_list<std::list<Type>> {
  static constexpr bool value = true;
};
/**
 * @brief проверка подстановки строки
 *
 * @tparam ypename
 */
template <typename> struct is_std_string {
  static constexpr bool value = false;
};
/**
 * @brief проверка подстановки строки
 *
 * @tparam
 */
template <> struct is_std_string<std::string> {
  static constexpr bool value = true;
};

/**
 * @brief Вывод значения адреса для контейнеров в стандартной библиотке
 *
 * @tparam T шаблонный тип
 * @param ip адрес
 * @return typename std::enable_if<is_std_vector<T>::value ||
 * is_std_list<T>::value>::type
 */
template <typename T>
typename std::enable_if<is_std_vector<T>::value || is_std_list<T>::value>::type
print_ip(const T &ip) {
  auto it = ip.begin();
  auto size = ip.size() - 1;
  for (size_t i = 0; i < size; ++i, ++it) {
    std::cout << *it << ".";
  }
  std::cout << *it << "\n";
}
/**
 * @brief Вывод строкового представления адреса
 *
 * @param ip адрес
 */
template <typename T>
typename std::enable_if<is_std_string<T>::value>::type print_ip(const T &ip) {
  std::cout << ip << "\n";
}
namespace impl {
/**
 * @brief Шаблон для конвертации типов
 *
 * @param ip
 * @return * template <typename T>
 */
template <typename T> T print_ip(const T &ip) { return ip; }

/**
 * @brief Вывод значения дреса представленного кортежем из одинаковых типов
 *
 * @tparam Tuple тип кортежа
 * @tparam k количество элементов
 * @param ip сам адрес
 */
template <typename Tuple, size_t... k>
void print_ip(Tuple ip, std::index_sequence<k...>) {
  auto val = {print_ip(std::get<k>(ip))...};
  auto it = std::begin(val);
  auto end = it + val.size() - 1;
  for (; it != end; ++it) {
    std::cout << std::to_string(*it) << ".";
  }
  std::cout << std::to_string(*it) << "\n";
  return;
}
}; // namespace impl
/**
 * @brief Вывод адреса из кортежа
 *
 * @tparam Args аргументы шаблона
 * @param ip адрес
 */
template <typename... Args> void print_ip(const std::tuple<Args...> &ip) {
  constexpr auto size = std::tuple_size<std::tuple<Args...>>::value;
  return impl::print_ip(ip, std::make_index_sequence<size>{});
}
