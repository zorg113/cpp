#include "lib.h"

#include "version.h"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using strings = std::vector<std::string>;
using ip = std::array<int, 4>;
using string = std::string;
using ssize_type = std::string::size_type;

enum num_byte { any_byte = -1, first, second, third, fourth };

/**
 * @brief  вывод версии
 *
 * @return int номер версии
 */
int version() { return PROJECT_VERSION_PATCH; }

/**
 * @brief  разделение строки на подстроки
 *
 * @param str входная строка
 * @param delim  разделитель
 * @return strings массив подстрок
 */
strings split(const string &str, char delim) {
  strings r;
  ssize_type start = 0;
  ssize_type stop = str.find_first_of(delim);
  while (stop != std::string::npos) {
    r.push_back(str.substr(start, stop - start));
    start = stop + 1;
    stop = str.find_first_of(delim, start);
  }
  r.push_back(str.substr(start));
  return r;
}

/**
 * @brief  преобразование строкового представления ipv4
 *         в array<int,4>
 * @param str входная строка
 * @param delim  разделитель
 * @return ip (array<int,4)
 */
ip string_to_ip(const string &str, char delim) {
  ip r;
  auto sbyte = split(str, delim);
  for (size_t i = 0; i < 4; ++i) {
    r[i] = std::stoi(sbyte[i]);
  }
  return r;
}
/**
 * @brief  печать адреса
 *
 * @param addr ip адрес
 */
void print_adress(const ip &addr) {
  bool flag = false;
  for (auto &s : addr) {
    if (flag)
      std::cout << "." << s;
    else
      std::cout << s;
    flag = true;
  }
  std::cout << std::endl;
}

/**
 * @brief печать пула адресов
 *
 * @param vs входной массив
 */
void print_pool(const std::vector<ip> &vs) {
  for (auto &a : vs) {
    print_adress(a);
  }
}
/**
 * @brief лексикографичексая сортировка
 *
 * @param ip_pool  пул адресов
 */
void lex_sort(std::vector<ip> &ip_pool) {
  std::sort(ip_pool.begin(), ip_pool.end(),
            [](const auto &ip1, const auto ip2) {
              const auto size = std::tuple_size<ip>{};
              for (size_t i = 0; i < size; ++i) {
                if (ip1[i] < ip2[i])
                  return false;
                else if (ip1[i] > ip2[i])
                  return true;
              }
              return true;
            });
}

/**
 * @brief печать ip адреса по значению байта
 *
 * @param vs пул адресов
 * @param value значение
 * @param nb номер байта num_byte
 */
void print_byte_eq(const std::vector<ip> &vs, const int value, num_byte nb) {
  for (auto &a : vs) {
    if (nb != any_byte) {
      if (a[nb] == value) {
        print_adress(a);
      }
    } else {
      for (auto &b : a) {
        if (b == value) {
          print_adress(a);
        }
      }
    }
  }
}
/**
 * @brief печать адресов с заданным значением первого и второго байта
 *
 * @param vs пул адресов
 * @param first  значение первого байта
 * @param second  значение второго байта
 */
void print_first_second_eq(const std::vector<ip> &vs, const int first,
                           const int second) {
  for (auto &a : vs) {
    if (a[0] == first && a[1] == second) {
      print_adress(a);
    }
  }
}

/**
 * @brief чтение, фильтрация ip адресов
 *
 */
void parse_ip() {
  std::ifstream in("./../ip_filter.tsv");
  std::cin.rdbuf(in.rdbuf());
  std::vector<ip> ip_pool;
  for (string line; std::getline(std::cin, line);) {
    strings strs = split(line, '\t');
    ip_pool.push_back(string_to_ip(strs.at(0), '.'));
  }
  // сортировка
  lex_sort(ip_pool);
  print_pool(ip_pool);
  // первый байт равен 1
  print_byte_eq(ip_pool, 1, first);
  // первый равен 46 второй 70
  print_first_second_eq(ip_pool, 46, 70);
  // любой байт равне 46
  print_byte_eq(ip_pool, 46, any_byte);
}
