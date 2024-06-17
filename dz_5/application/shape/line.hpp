#pragma once
#include "point.hpp"
#include "shape.hpp"
namespace editor {
class Line : public IShape {
public:
  Line(Point first, Point last) : m_first(first), m_last(last) {}
  Line(const Line &) = default;
  Line(Line &&) = default;
  ~Line() override = default;
  Line &operator=(const Line &) = default;
  Line &operator=(Line &&) = default;
  Ptr clone() override { return std::make_unique<Line>(m_first, m_last); }
  void render(Canvas &canvas) override {
    canvas << " Rectangle{ Fist  " << m_first << ", Last " << m_last << "}";
  }

private:
  Point m_first;
  Point m_last;
};
} // namespace editor