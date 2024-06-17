#pragma once
#include "point.hpp"
#include "shape.hpp"
namespace editor {
class Rectangle : public IShape {
public:
  Rectangle(Point upleft, int width, int height)
      : m_upleft(upleft), m_width(width), m_height(height) {}

  Rectangle(const Rectangle &) = default;
  Rectangle(Rectangle &&) = default;
  ~Rectangle() override = default;
  Rectangle &operator=(const Rectangle &) = default;
  Rectangle &operator=(Rectangle &&) = default;
  Ptr clone() override {
    return std::make_unique<Rectangle>(m_upleft, m_width, m_height);
  }

  void render(Canvas &canvas) override {
    canvas << " Rectangle{ UpLeft  " << m_upleft << ", width " << m_width
           << ", height" << m_height << "}";
  }

private:
  Point m_upleft;
  int m_width, m_height;
};
} // namespace editor