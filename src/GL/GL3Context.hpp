#pragma once

#include "core/RenderContext.hpp"

namespace fui {

class GL3Context : public RenderContext {
public:
  explicit GL3Context(NVGcontext* vg);
  ~GL3Context();
};

} // namespace fui
