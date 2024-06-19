#pragma once
#include "point.hpp"
#include "shape.hpp"
namespace editor {
/**
 * @brief class Line
 * represents a line
 */
class Line : public IShape {
public:
  /**
   * @brief Construct a new Line object
   *
   * @param first start of line
   * @param last  end of line
   */
  Line(Point first, Point last) : m_first(first), m_last(last) {}
  Line(const Line &) = default;
  Line(Line &&) = default;
  ~Line() override = default;
  Line &operator=(const Line &) = default;
  Line &operator=(Line &&) = default;
  Ptr clone() override { return std::make_unique<Line>(m_first, m_last); }
  /**
   * @brief  draw object on canvas
   *
   * @param canvas
   */
  void draw(Canvas &canvas) override {
    canvas << " Rectangle{ Fist  " << m_first << ", Last " << m_last << "}\n";
  }

private:
  Point m_first;
  Point m_last;
};
} // namespace editor