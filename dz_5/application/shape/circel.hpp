#pragma once
#include "point.hpp"
#include "shape.hpp"
namespace editor {
/**
 * @brief class Circel
 *
 */
class Circel : public IShape {
public:
  /**
   * @brief Construct a new Circel object
   *
   * @param center  center point
   * @param radius radius of circle
   */
  Circel(Point center, int radius) : m_center(center), m_radius(radius) {}
  Circel(const Circel &) = default;
  Circel(Circel &&) = default;
  ~Circel() override = default;
  Circel &operator=(const Circel &) = default;
  Circel &operator=(Circel &&) = default;
  Ptr clone() override { return std::make_unique<Circel>(m_center, m_radius); }
  /**
   * @brief  draw object on canvas
   *
   * @param canvas
   */
  void draw(Canvas &canvas) override {
    canvas << " Circel { Center  " << m_center << ", raduis " << m_radius
           << "}\n";
  }

private:
  Point m_center;
  int m_radius;
};
} // namespace editor