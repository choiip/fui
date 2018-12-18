#include "GL/GL3Context.hpp"
#include <iostream>
#include <GL/glad.h>
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "core/Status.hpp"

namespace fui {

GL3Context::GL3Context() {}

GL3Context::~GL3Context() { nvgDeleteGL3(vg()); }

Status GL3Context::initVG() {
  if (_vg != nullptr) {
    nvgDeleteGL3(_vg);
  }
  int flag = NVG_ANTIALIAS | NVG_STENCIL_STROKES;
#ifdef NDEBUG
  flag |= NVG_DEBUG;
#endif  
  _vg = nvgCreateGL3(flag);
  if (_vg == NULL) {
    std::cerr << ("Could not init nanovg (GL3).\n");
    return Status::UNKNOWN_ERROR;
  }
  return Status::OK;
}

} // namespace fui
