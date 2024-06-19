#pragma once
#include "./../shape/shape.hpp"
#include <assert.h>
#include <unordered_set>

#include "./../shape/circel.hpp"
#include "./../shape/line.hpp"
#include "./../shape/point.hpp"
#include "./../shape/rectangle.hpp"

namespace editor {
/**
 * @brief class Document
 * Represents shape
 */
class Document : public IRenderable {
public:
  using Ptr = std::unique_ptr<Document>;
  Document() = default;
  virtual ~Document() = default;
  /**
   * @brief add line to document
   *
   * @param start start of line
   * @param end end of line
   * @return const Line::Ptr&
   */
  const Line::Ptr &add_line(Point start, Point end) {
    auto [it, is_inserted] =
        m_shapes.emplace(std::make_unique<Line>(start, end));
    assert(is_inserted);
    shape_render((*it).get());
    return *it;
  }
  /**
   * @brief add rectangle to document
   *
   * @param upleft  upper left point
   * @param width   width rectangle
   * @param height  height rectangle
   * @return const Rectangle::Ptr&
   */
  const Rectangle::Ptr &add_rectangle(Point upleft, int width, int height) {
    auto [it, is_inserted] =
        m_shapes.emplace(std::make_unique<Rectangle>(upleft, width, height));
    assert(is_inserted);
    shape_render((*it).get());
    return *it;
  }
  /**
   * @brief add circel to document
   *
   * @param center center point
   * @param radius radius of circle
   * @return const Circel::Ptr&
   */
  const Circel::Ptr &add_circel(Point center, int radius) {
    auto [it, is_inserted] =
        m_shapes.emplace(std::make_unique<Circel>(center, radius));
    assert(is_inserted);
    shape_render((*it).get());
    return *it;
  }
  /**
   * @brief draw all shapes
   *
   * @param canvas referecs to canvas
   */
  void draw(Canvas &canvas) override {
    for (const auto &elem : m_shapes) {
      elem->draw(canvas);
    }
  }
  /**
   * @brief remove shape from document
   *
   * @param shape shape interface
   * @return true if remove successfully
   * @return false otherwise
   */
  bool remove_shape(const IShape::Ptr &shape) {
    std::cout << "remove_shape" << std::endl;
    shape->notify_render();
    return true;
  }

private:
  /**
   * @brief send command to view redraw
   *
   * @param shape
   */
  void shape_render(IRenderable *shape) {
    shape->add_renders(m_renders);
    shape->notify_render();
  }
  std::unordered_set<IShape::Ptr> m_shapes;
};
} // namespace editor