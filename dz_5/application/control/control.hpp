#pragma once

#include "./../document/document.hpp"
#include "./../serialize/serialize.hpp"
#include "./../view/view.hpp"

#include <cassert>
#include <memory>
#include <unordered_set>

namespace editor {

/**
 * @brief class Control
 * Controller document and view
 */

class Control {
public:
  using Ptr = std::unique_ptr<Control>;
  Control(View::Ptr &view) : m_view(view){};
  ~Control() = default;
  /**
   * @brief Create a document object
   *
   * @return const Document::Ptr&
   */
  const Document::Ptr &create_document() {
    auto [it, is_inserted] = m_documents.emplace(std::make_unique<Document>());
    assert(is_inserted);
    document_render((*it).get());
    return *it;
  }
  /**
   * @brief  import document from  object with serialize interface
   *
   * @param from
   * @return const Document::Ptr&
   */
  const Document::Ptr &import_document(const ISerialize::Ptr &from) {
    auto [it, is_inserted] = m_documents.emplace(from->import_document());
    assert(is_inserted);
    document_render((*it).get());
    return *it;
  }
  /**
   * @brief export document to object with serialize interface
   *
   * @param doc
   * @param to
   */
  void export_document(const Document::Ptr &doc, const ISerialize::Ptr &to) {
    to->export_document(doc);
  }
  /**
   * @brief close document
   *
   * @param doc
   * @return true if successfully
   * @return false otherwise
   */

  bool close_document(const Document::Ptr &doc) {
    std::size_t is_close = m_documents.erase(doc);
    return is_close == 0 ? false : true;
  }

private:
  /**
   * @brief render document to view
   *
   * @param doc
   */
  void document_render(IRenderable *doc) {
    doc->add_render(m_view);
    doc->notify_render();
  }
  std::unordered_set<Document::Ptr> m_documents;
  View::Ptr &m_view;
};
} // namespace editor