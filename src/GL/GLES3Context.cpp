#include "GL/GLES3Context.hpp"
#include <GL/header.h>
#define NANOVG_GLES3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "core/Log.hpp"
#include "core/MathDef.hpp"
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

auto GLES3Context::setViewport(int x, int y, int width, int height) -> decltype(this) {
  glViewport(x, y, width, height);
  return this;
}

auto GLES3Context::preDraw(const Color* clearColor, const float* clearDepth, const int* clearStencil) -> decltype(this) {
  GLbitfield clearBits = 0;
  if (clearColor != nullptr) {
    glClearColor(clearColor->r, clearColor->g, clearColor->b, clearColor->a);
    clearBits |= GL_COLOR_BUFFER_BIT;
  }
  if (clearDepth != nullptr) {
    glClearDepthf(*clearDepth);
    clearBits |= GL_DEPTH_BUFFER_BIT;
  }
  if (clearStencil != nullptr) {
    glClearStencil(*clearStencil);
    clearBits |= GL_STENCIL_BUFFER_BIT;
  }
  if (clearBits != 0) {
    glClear(clearBits);
  }
  return this;
}

auto GLES3Context::postDraw() -> decltype(this) {
  return this;
}

std::string GLES3Context::versionLine() const {
  return "#version 300 es";
}

std::string GLES3Context::vertexMacro() const {
  return R"(
    #define VERT_IN in
    #define VERT_OUT out
  )";
}

std::string GLES3Context::fragmentMacro() const {
  return R"(
		precision highp float;
    #define FRAG_IN in
    #define FRAG_OUT out
  )";
}

} // namespace fui
