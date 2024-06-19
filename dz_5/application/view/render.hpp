#pragma once
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>
namespace editor {

using Canvas = std::ostream;
class IRenderable;
using IRenderable_Ptr = std::unique_ptr<IRenderable>;
/**
 * @brief interface render object
 *
 */
class IRender {
public:
  using Ptr = std::shared_ptr<IRender>;
  IRender() = default;
  virtual ~IRender() = default;
  virtual void render(IRenderable *rnd) = 0;
};
/**
 * @brief interface renderable object
 *
 */
class IRenderable {
public:
  virtual ~IRenderable() = default;
  /**
   * @brief add array available renders for a renderable object
   *
   * @param renders
   */
  void add_renders(std::vector<IRender::Ptr> &renders) {
    m_renders.insert(m_renders.end(), renders.begin(), renders.end());
  }
  /**
   * @brief add reder to renderable
   *
   * @param render
   */
  void add_render(const IRender::Ptr &render) { m_renders.push_back(render); }
  /**
   * @brief signal for rendering
   *
   */
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