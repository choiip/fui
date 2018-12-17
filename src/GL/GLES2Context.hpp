#pragma once

#include "core/RenderContext.hpp"

namespace fui {

enum class Status;

class GLES2Context : public RenderContext {
public:
  GLES2Context();
  ~GLES2Context();

  Status initVG();
};

} // namespace fui
