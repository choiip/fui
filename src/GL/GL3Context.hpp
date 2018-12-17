#pragma once

#include "core/RenderContext.hpp"

namespace fui {

enum class Status;

class GL3Context : public RenderContext {
public:
  GL3Context();
  ~GL3Context();

  Status initVG();
};

} // namespace fui
