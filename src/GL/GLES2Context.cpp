#include "GL/GLES2Context.hpp"
#include <iostream>
#include <GL/glad.h>
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"
#include "core/Status.hpp"

namespace fui {

GLES2Context::GLES2Context() {}

GLES2Context::~GLES2Context() { nvgDeleteGLES2(_vg); }

Status GLES2Context::initVG() {
  if (_vg != nullptr) {
    nvgDeleteGLES2(_vg);
  }
  int flag = NVG_ANTIALIAS | NVG_STENCIL_STROKES;
#ifdef NDEBUG
  flag |= NVG_DEBUG;
#endif
  _vg = nvgCreateGLES2(flag);
  if (_vg == NULL) {
    std::cerr << ("Could not init nanovg (GLES2).\n");
    return Status::UNKNOWN_ERROR;
  }
  return Status::OK;
}

} // namespace fui
