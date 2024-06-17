#pragma once
#include <memory>
#include <render.hpp>

namespace editor {

class IShape : public IRender {
public:
  using Ptr = std::unique_ptr<IShape>;
  virtual ~IShape() = default;
  virtual Ptr clone() = 0;
};

} // namespace editor