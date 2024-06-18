#pragma once

#include "./../document/document.hpp"
#include "./../serialize/serialize.hpp"
#include "./../view/view.hpp"

#include <cassert>
#include <memory>
#include <unordered_set>

namespace editor {

class Control {
public:
  using Ptr = std::unique_ptr<Control>;
  Control() = default;
  ~Control() = default;
  const Document::Ptr &create_document(const ::editor::View::Ptr &view) {
    auto [it, is_inserted] = m_documents.emplace(std::make_unique<Document>());
    assert(is_inserted);
    (*it)->add_render(view);
    (*it)->notify_render();
    return *it;
  }
  const Document::Ptr &import_document(const ISerialize::Ptr &from,
                                       const ::editor::View::Ptr &view) {
    auto [it, is_inserted] = m_documents.emplace(from->import_document());
    assert(is_inserted);
    (*it)->add_render(view);
    (*it)->notify_render();
    return *it;
  }
  void export_document(const Document::Ptr &doc, const ISerialize::Ptr &to) {
    to->export_document(doc);
  }

  bool close_document(const Document::Ptr &doc) {
    std::size_t is_close = m_documents.erase(doc);
    return is_close == 0 ? false : true;
  }

private:
  std::unordered_set<Document::Ptr> m_documents;
};
} // namespace editor