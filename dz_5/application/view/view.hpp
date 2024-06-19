#pragma once

// #include "./../document/document.hpp"
#include "render.hpp"
#include <iostream>

namespace editor {
/**
 * @brief class View
 *
 */
class View : public IRender {
public:
  using Ptr = std::shared_ptr<View>;
  void render(IRenderable *object) override { object->draw(m_canvas); }

private:
  Canvas &m_canvas = std::cout;
};
} // namespace editor