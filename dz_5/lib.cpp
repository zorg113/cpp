#include "lib.h"
#include <string>

#include "version.h"

#include "application/control/control.hpp"

int version() { return PROJECT_VERSION_PATCH; }

void run_mvc() {
  editor::ISerialize::Ptr file_io =
      std::make_unique<editor::FileIo>("test.dat"s);
  editor::View::Ptr view = std::make_unique<editor::View>();
  editor::Control::Ptr control = std::make_unique<editor::Control>(view);
  const auto &doc1 = control->import_document(file_io);
  const auto &doc2 = control->create_document();
  const doc2->add_circel(editor::Point{10, 10}, 11);
}
