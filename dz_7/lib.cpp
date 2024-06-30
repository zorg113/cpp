#include "lib.h"

#include "parse_cmd.hpp"
#include "version.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

/**
 * @brief  вывод версии
 *
 * @return int номер версии
 */
int version() { return PROJECT_VERSION_PATCH; }
/**
 * @brief работа с матрицей
 *
 */
void run_matrix() {
  mt::matrix<int, 0> test_m;
  const size_t size = 9;
  for (size_t i = 0; i <= size; ++i) {
    test_m[i][i] = i;
    test_m[i][size - i] = size - i;
  }
  for (auto i = 1; i <= 8; ++i) {
    for (auto j = 1; j <= 8; ++j)
      std::cout << test_m[i][j] << " ";
  }
  std::cout << std::endl;
  std::cout << test_m.size() << std::endl;
  for (auto it : test_m) {
    int x, y, v;
    std::tie(x, y, v) = it;
    std::cout << "[" << x << "]"
              << "[" << y << "] val " << v << std::endl;
  }
  mt::matrix<int, 0, 3> test_3d;
  for (size_t i = 0; i <= size; ++i) {
    for (size_t j = 0; j <= size; ++j) {
      test_3d[i][i][j] = i;
      test_3d[i][size - i][j] = size - i;
    }
  }
  std::cout << test_3d.size() << std::endl;
  std::cout << test_3d[5][5][5] << std::endl;
  ((test_3d[5][5][5] = 10) = 111) = 999;
  std::cout << test_3d[5][5][5] << std::endl;
}
