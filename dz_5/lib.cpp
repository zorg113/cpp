#include "lib.h"

#include "application/serialize/serialize.hpp"
#include "version.h"

#include "application/control/control.hpp"
#include "application/view/view.hpp"
#include <memory>

int version() { return PROJECT_VERSION_PATCH; }

void run_mvc() {
  std::string filename = "test.dat";
  editor::ISerialize::Ptr file_io = std::make_unique<editor::FileIo>(filename);
  auto view = std::make_shared<editor::View>();
  auto control = std::make_unique<editor::Control>(view);
  const auto &doc_new = control->create_document();
  const auto &doc_import = control->import_document(file_io);
  auto &circel = doc_new->add_circel(editor::Point{10, 10}, 11);
  doc_new->remove_shape(circel);
  control->export_document(doc_new, file_io);
}
