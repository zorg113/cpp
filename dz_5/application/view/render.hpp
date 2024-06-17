#pragma once
#include <memory>
#include <ostream>
namespace editor {

using Canvas = std::ostream;

class IRender {
public:
  using Ptr = std::unique_ptr<IRender>;
  IRender() = default;
  virtual ~IRender() = default;
  virtual void render(Canvas &canvas) = 0;
};
} // namespace editor