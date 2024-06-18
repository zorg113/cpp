#pragma once
#include "./../document/document.hpp"
#include <iostream>
namespace editor {
class ISerialize {
public:
  using Ptr = std::unique_ptr<ISerialize>;
  virtual Document::Ptr import_document() = 0;
  virtual void export_document(const Document::Ptr &doc) = 0;
};

class FileIo : public ISerialize {
public:
  using Ptr = std::unique_ptr<FileIo>;
  FileIo(const std::string &filename) : m_filename(filename) {}
  ~FileIo() = default;
  FileIo() = delete;
  Document::Ptr import_document() override {
    return std::make_unique<Document>();
  }

  void export_document(const Document::Ptr &doc) override {
    std::cout << "save to file" << m_filename << "\n";
  }

private:
  std::string m_filename;
};

} // namespace editor