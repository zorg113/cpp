#pragma once
#include "./../shape/shape.hpp"
#include <assert.h>
#include <unordered_set>

#include "./../shape/circel.hpp"
#include "./../shape/line.hpp"
#include "./../shape/point.hpp"
#include "./../shape/rectangle.hpp"

namespace editor {

class Document : public IRenderable {
public:
  using Ptr = std::unique_ptr<Document>;
  Document() = default;
  virtual ~Document() = default;

  void add_line(Point start, Point end) {
    auto [it, is_inserted] =
        m_shapes.emplace(std::make_unique<Line>(start, end));
    assert(is_inserted);
    (*it)->add_renders(m_renders);
    (*it)->notify_render();
  }
  void add_rectangle(Point upleft, int width, int height) {
    auto [it, is_inserted] =
        m_shapes.emplace(std::make_unique<Rectangle>(upleft, width, height));
    assert(is_inserted);
    (*it)->add_renders(m_renders);
    (*it)->notify_render();
  }
  void add_circel(Point center, int radius) {
    auto [it, is_inserted] =
        m_shapes.emplace(std::make_unique<Circel>(center, radius));
    assert(is_inserted);
    (*it)->add_renders(m_renders);
    (*it)->notify_render();
  }
  void draw(Canvas &canvas) override {
    for (const auto &elem : m_shapes) {
      elem->draw(canvas);
    }
  }
  bool remove_shape(const IShape &shape) { return true; }

private:
  std::unordered_set<IShape::Ptr> m_shapes;
};
} // namespace editor