#pragma once

#include <document.hpp>
#include <serialize.hpp>
#include <view.hpp>

namespace editor {

class Control {
public:
  using Ptr = std::unique_ptr<Control>;
  Control(View::Ptr p_view) : m_view(p_view) {}
  ~Control() = default;
  Control() = delete;
  const Document::Ptr &create_document() {
    auto [it, is_inserted] = m_documents.emplace(std::make_unique<Document>());
    assert(is_inserted);
    return *it;
  }
  const Document::Ptr &import_document(const ISerialize::Ptr &from) {
    auto [it, is_inserted] = m_documents.emplace(from->import_document());
    assert(is_inserted);
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
  std::unique_ptr<View> &m_view;
};
} // namespace editor