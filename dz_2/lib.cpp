#include "lib.h"

#include "version.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using strings = std::vector<std::string>;
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
 * @brief  печать адреса
 *
 * @param addr ip адрес
 */
void print_adress(const strings &addr) {
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
void print_pool(const std::vector<strings> &vs) {
  for (auto &a : vs) {
    print_adress(a);
  }
}
/**
 * @brief лексикографичексая сортировка
 *
 * @param ip_pool  пул адресов
 */
void lex_sort(std::vector<strings> &ip_pool) {
  std::sort(ip_pool.begin(), ip_pool.end(),
            [](const auto &ip1, const auto ip2) {
              if (std::stoi(ip1[0]) < std::stoi(ip2[0]))
                return false;
              if (std::stoi(ip1[0]) == std::stoi(ip2[0])) {
                if (std::stoi(ip1[1]) < std::stoi(ip2[1]))
                  return false;
                if (std::stoi(ip1[1]) == std::stoi(ip2[1])) {
                  if (std::stoi(ip1[2]) < std::stoi(ip2[2]))
                    return false;
                  if (std::stoi(ip1[2]) == std::stoi(ip2[2])) {
                    if (std::stoi(ip1[3]) < std::stoi(ip2[3]))
                      return false;
                  }
                }
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
void print_byte_eq(const std::vector<strings> &vs, const std::string value,
                   num_byte nb) {
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
void print_first_second_eq(const std::vector<strings> &vs,
                           const std::string first, const std::string second) {
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
  std::vector<strings> ip_pool;
  for (string line; std::getline(std::cin, line);) {
    strings strs = split(line, '\t');
    ip_pool.push_back(split(strs.at(0), '.'));
  }
  // сортировка
  lex_sort(ip_pool);
  print_pool(ip_pool);
  // первый байт равен 1
  print_byte_eq(ip_pool, "1", first);
  // первый равен 46 второй 70
  print_first_second_eq(ip_pool, "46", "70");
  // любой байт равне 46
  print_byte_eq(ip_pool, "46", any_byte);
}
