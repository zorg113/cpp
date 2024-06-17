#pragma once

#include <document.hpp>
#include <iostream>

namespace editor {
class View {
public:
  using Ptr = std::unique_ptr<View>;
  void show_documents(const Documents &docs) {
    for (auto &doc : docs) {
      doc->render(m_canvas);
    }
  }
  void show(const IRender::Ptr &render) { render->render(m_canvas); }

private:
  Canvas &m_canvas = std::cout;
};
} // namespace editor