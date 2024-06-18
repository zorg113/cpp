#pragma once
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>
namespace editor {

using Canvas = std::ostream;
class IRenderable;
using IRenderable_Ptr = std::unique_ptr<IRenderable>;
class IRender {
public:
  using Ptr = std::shared_ptr<IRender>;
  IRender() = default;
  virtual ~IRender() = default;
  virtual void render(IRenderable *rnd) = 0;
};

class IRenderable {
public:
  virtual ~IRenderable() = default;
  void add_renders(std::vector<IRender::Ptr> &renders) {
    m_renders.insert(m_renders.end(), renders.begin(), renders.end());
  }
  void add_render(const IRender::Ptr &render) { m_renders.push_back(render); }
  void notify_render() {
    for (auto &r : m_renders) {
      r->render(this);
    }
  }
  virtual void draw(Canvas &canvas) = 0;

protected:
  std::vector<IRender::Ptr> m_renders;
};
} // namespace editor