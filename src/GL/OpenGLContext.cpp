#include "GL/OpenGLContext.hpp"
#include <GL/header.h>
#include <stb.h>
#include "core/MathDef.hpp"

namespace fui {

OpenGLContext::OpenGLContext() = default;
OpenGLContext::~OpenGLContext() = default;

std::string OpenGLContext::loadVertexShader(const std::string& filename) {
  size_t contentLength;
  char* content = (char*)stb_file((char*)filename.c_str(), &contentLength);
  if (content == NULL) return "";
  content[contentLength - 1] = 0;

  std::string finalSource = versionLine();
  finalSource += vertexMacro();
  finalSource += content;
  free(content);
  return finalSource;
}

std::string OpenGLContext::loadFragmentShader(const std::string& filename) {
  size_t contentLength;
  char* content = (char*)stb_file((char*)filename.c_str(), &contentLength);
  if (content == NULL) return "";
  content[contentLength - 1] = 0;

  std::string finalSource = versionLine();
  finalSource += fragmentMacro();
  finalSource += content;
  free(content);
  return finalSource;
}

auto OpenGLContext::setViewport(int x, int y, int width, int height) -> decltype(this) {
  glViewport(x, y, width, height);
  return this;
}

auto OpenGLContext::preDraw(const Recti& renderArea, const Color* clearColor, const float* clearDepth,
                            const int* clearStencil) -> decltype(this) {
  setViewport(renderArea.x, renderArea.y, renderArea.w, renderArea.h);
  GLbitfield clearBits = 0;
  if (clearColor != nullptr) {
    glClearColor(clearColor->r, clearColor->g, clearColor->b, clearColor->a);
    clearBits |= GL_COLOR_BUFFER_BIT;
  }
  if (clearDepth != nullptr) {
    glClearDepth(*clearDepth);
    clearBits |= GL_DEPTH_BUFFER_BIT;
  }
  if (clearStencil != nullptr) {
    glClearStencil(*clearStencil);
    clearBits |= GL_STENCIL_BUFFER_BIT;
  }
  if (clearBits != 0) { glClear(clearBits); }
  return this;
}

auto OpenGLContext::postDraw() -> decltype(this) { return this; }

} // namespace fui
