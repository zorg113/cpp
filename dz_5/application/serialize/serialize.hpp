#pragma once
#include "./../document/document.hpp"
#include <iostream>
namespace editor {
/**
 * @brief class ISerialize
 *  interface class serializer
 */
class ISerialize {
public:
  using Ptr = std::unique_ptr<ISerialize>;
  /**
   * @brief import documet data
   *
   * @return Document::Ptr
   */
  virtual Document::Ptr import_document() = 0;
  /**
   * @brief export document data
   *
   * @param doc pointer to document
   */
  virtual void export_document(const Document::Ptr &doc) = 0;
};
/**
 * @brief class FileIo serializer
 *
 */
class FileIo : public ISerialize {
public:
  using Ptr = std::unique_ptr<FileIo>;
  FileIo(const std::string &filename) : m_filename(filename) {}
  ~FileIo() = default;
  FileIo() = delete;
  /**
   * @brief import documet data from file
   *
   * @return Document::Ptr
   */
  Document::Ptr import_document() override {
    std::cout << "load from file" << m_filename << "\n";
    return std::make_unique<Document>();
  }
  /**
   * @brief export document data to file
   *
   * @param doc pointer to document
   */
  void export_document(const Document::Ptr &doc) override {
    std::cout << "save to file" << m_filename << "\n";
  }

private:
  std::string m_filename;
};

} // namespace editor