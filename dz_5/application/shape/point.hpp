#pragma once
#include <ostream>

namespace editor {
/**
 * @brief Struct Point
 *  point on a two-dimensional plane
 */
struct Point {
  int x;
  int y;
};

inline std::ostream &operator<<(std::ostream &stream, Point point) {
  stream << "Point{ x = " << point.x << " y = " << point.y << "}";
  return stream;
}

} // namespace editor