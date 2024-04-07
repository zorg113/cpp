#include "lib.h"

#include "best_allocator.h"
#include "best_vector.h"
#include "version.h"
#include <map>
#include <vector>

int version() { return PROJECT_VERSION_PATCH; }

int factorial(int n) {
  if (n == 0 || n == 1)
    return 1;
  return n * factorial(n - 1);
}

template <typename T> void print_map(T &mp) {
  for (const auto &[key, value] : mp) {
    std::cout << key << " " << value << "\n";
  }
}

void custom_allocator() {
  constexpr size_t max_size = sizeof(int) * 100;
  std::map<int, int> std_map;
  std::map<int, int, std::less<int>, best::bestalloc<int, max_size>> test_map;
  std::vector<int, best::bestalloc<int, max_size>> test_vec;
  try {
    for (size_t i = 0; i < 10; ++i) {
      std_map[i] = factorial(i);
    }
    for (size_t i = 0; i < 10; ++i) {
      test_map[i] = factorial(i);
    }
    std::cout << "best allocator with map\n";
    print_map(test_map);

  } catch (std::bad_alloc &e) {

    std::cerr << e.what() << "\n";
  }
}
