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
  auto control = std::make_unique<editor::Control>();
  const auto &doc1 = control->import_document(file_io, view);
  const auto &doc2 = control->create_document(view);
  doc1->add_circel(editor::Point{10, 10}, 11);
}
