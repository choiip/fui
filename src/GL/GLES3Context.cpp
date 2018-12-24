#include "GL/GLES3Context.hpp"
#include <GL/glad.h>
#define NANOVG_GLES3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "core/Log.hpp"
#include "core/Status.hpp"

namespace fui {

GLES3Context::GLES3Context() {}

GLES3Context::~GLES3Context() { nvgDeleteGLES3(_vg); }

Status GLES3Context::initVG() {
  if (_vg != nullptr) {
    nvgDeleteGLES3(_vg);
  }
  int flag = NVG_ANTIALIAS | NVG_STENCIL_STROKES;
#ifdef NDEBUG
  flag |= NVG_DEBUG;
#endif  
  _vg = nvgCreateGLES3(flag);
  if (_vg == NULL) {
    LOGE << "Could not init nanovg (GLES3).";
    return Status::UNKNOWN_ERROR;
  }
  return Status::OK;
}

} // namespace fui
