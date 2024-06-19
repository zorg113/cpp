#pragma once
#include "point.hpp"
#include "shape.hpp"
namespace editor {
/**
 * @brief class Rectangle
 *  represents a rectangle
 */
class Rectangle : public IShape {
public:
  /**
   * @brief Construct a new Rectangle object
   *
   * @param upleft  The upper left coordinate
   * @param width   the width of the rectangle
   * @param height  the height of the rectangle
   */
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
  /**
   * @brief draw the rectangle on canvas
   *
   * @param canvas
   */
  void draw(Canvas &canvas) override {
    canvas << " Rectangle{ UpLeft  " << m_upleft << ", width " << m_width
           << ", height" << m_height << "}\n";
  }

private:
  Point m_upleft;
  int m_width, m_height;
};
} // namespace editor