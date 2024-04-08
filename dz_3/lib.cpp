#include "lib.h"

#include "best_allocator.h"
#include "best_vector.h"
#include "version.h"
#include <iostream>
#include <map>
#include <vector>
/**
 * @brief функция вывода версии
 *
 * @return int
 */
int version() { return PROJECT_VERSION_PATCH; }

/**
 * @brief рекурсивный расчет факториала
 *
 * @param n число
 * @return int факториал
 */
int factorial(int n) {
  if (n == 0 || n == 1)
    return 1;
  return n * factorial(n - 1);
}
/**
 * @brief печать в std::cout map
 *
 * @tparam T тип словаря
 * @param mp сам словарь
 */
template <typename T> void print_map(T &mp) {
  for (const auto &[key, value] : mp) {
    std::cout << key << " " << value << "\n";
  }
}

/**
 * @brief проверка распределителя памяти
 *
 */
void custom_allocator() {
  /// максимальный размер в байтах выделяемый аллокатором
  constexpr size_t max_size = sizeof(int) * 100;

  std::map<int, int> std_map;
  std::map<int, int, std::less<int>, best::bestalloc<int, max_size>> test_map;

  std::vector<int, best::bestalloc<int, max_size>> test_stl_vec;
  best::vector<int, best::bestalloc<int, max_size>> test_best_vec;
  try {
    //-----------------------------------
    for (size_t i = 0; i < 10; ++i) {
      std_map[i] = factorial(i);
    }
    for (size_t i = 0; i < 10; ++i) {
      test_map[i] = factorial(i);
    }
    std::cout << "best allocator with map\n";
    print_map(test_map);
    //-----------------------------------
    //   контенеры вектора
    for (size_t i = 0; i < 10; ++i) {
      test_best_vec.push_back(i);
      test_stl_vec.push_back(i);
    }
    std::cout << "best vec\t stl vec\n";
    for (size_t i = 0; i < 10; ++i) {
      std::cout << test_best_vec[i] << "\t" << test_stl_vec[i] << "\n";
    }

  } catch (std::bad_alloc &e) {

    std::cerr << e.what() << "\n";
  }
}
