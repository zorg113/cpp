#pragma once
#include "./../view/render.hpp"
#include <memory>

namespace editor {
/**
 * @brief interface shape
 *
 */
class IShape : public IRenderable {
public:
  using Ptr = std::unique_ptr<IShape>;
  virtual ~IShape() = default;
  virtual Ptr clone() = 0;
};

} // namespace editor