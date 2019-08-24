#include "GL/GLES2Context.hpp"
#include <GL/header.h>
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"
#include "core/Log.hpp"
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
    LOGE << "Could not init nanovg (GLES2).";
    return Status::UNKNOWN_ERROR;
  }
  return Status::OK;
}

auto GLES2Context::setViewport(int x, int y, int width, int height) -> decltype(this) {
  glViewport(x, y, width, height);
  return this;
}

std::string GLES2Context::versionLine() const {
  return "#version 100";
}

std::string GLES2Context::vertexMacro() const {
  return R"(
    #define VERT_IN attribute
    #define VERT_OUT varying
  )";
}

std::string GLES2Context::fragmentMacro() const {
  return R"(
  	#ifdef GL_FRAGMENT_PRECISION_HIGH
		  precision highp float;
		#else
		  precision mediump float;
		#endif
    #define FRAG_IN varying
  )";
}

} // namespace fui
