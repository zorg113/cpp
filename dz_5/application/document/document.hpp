#pragma once
#include <assert.h>
#include <shape.hpp>
#include <unordered_set>
#include <vector>

#include <circel.hpp>
#include <line.hpp>
#include <point.hpp>
#include <rectangle.hpp>

namespace editor {

using Documents = std::vector<Document>;
class Document : public IRender {
public:
  using Ptr = std::unique_ptr<Document>;
  Document() = default;
  virtual ~Document() = default;

  const IShape::Ptr &add_line(Point start, Point end) {
    auto [it, is_inserted] =
        m_shapes.emplace(std::make_unique<Line>(start, end));
    assert(is_inserted);
    return *it;
  }
  const IShape::Ptr &add_rectangle(Point upleft, int width, int height) {
    auto [it, is_inserted] =
        m_shapes.emplace(std::make_unique<Rectangle>(upleft, width, height));
    assert(is_inserted);
    return *it;
  }
  const IShape::Ptr &add_circel(Point center, int radius) {
    auto [it, is_inserted] =
        m_shapes.emplace(std::make_unique<Circel>(center, radius));
    assert(is_inserted);
    return *it;
  }
  void render(Canvas &canvas) override {
    for (const auto &elem : m_shapes) {
      elem->render(canvas);
    }
  }
  bool remove_shape(const IShape &shape) {}

private:
  std::unordered_set<IShape::Ptr> m_shapes;
};
} // namespace editor