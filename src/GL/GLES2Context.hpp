#pragma once

#include "core/RenderContext.hpp"

namespace fui {

class GLES2Context : public RenderContext {
public:
  GLES2Context(NVGcontext* vg);
  ~GLES2Context();
};

} // namespace fui
