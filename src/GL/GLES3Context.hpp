#pragma once

#include "core/RenderContext.hpp"

namespace fui {

enum class Status;

class GLES3Context : public RenderContext {
public:
  GLES3Context();
  ~GLES3Context();

  Status initVG();
};

} // namespace fui
