#include "GlfwGLES2Profile.hpp"
#ifndef __EMSCRIPTEN__
#include <GL/glad.h>
#define GLFW_INCLUDE_NONE
#else
#define GLFW_INCLUDE_ES2
#endif
#include <GLFW/glfw3.h>
#include <memory>
#include "core/Log.hpp"
#include "core/Status.hpp"
#include "GL/GLES2Context.hpp"
#include "GL/GLES3Context.hpp"

namespace fui {

void GlfwGLES2Profile::prepare() const {
  //// https://www.glfw.org/docs/latest/window_guide.html#window_hints
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#ifndef _WIN32 // don't require this on win32, and works with more cards
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
}

RenderContext* GlfwGLES2Profile::createContext(void* nativeWindow) const {
  GLFWwindow* window = (GLFWwindow*)nativeWindow;
  glfwMakeContextCurrent(window);
#ifndef __EMSCRIPTEN__
  //// https://emscripten.org/docs/optimizing/Optimizing-WebGL.html#optimizing-load-times-and-other-best-practices
  if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
    LOGE << "Could not initialize GLAD!";
    return nullptr;
  }
  glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
  auto glMajor = GLVersion.major;
#else
  const char* version = (const char*)glGetString(GL_VERSION);
  int glMajor = 0;
  sscanf(version, "OpenGL ES %d.", &glMajor);
#endif

  if (glMajor == 3) {
    std::unique_ptr<GLES3Context> context(new GLES3Context);
    if (!context) {
      LOGE << "Could not create render context.";
      return nullptr;
    }
    if (context->initVG() != Status::OK) { return nullptr; }
    return context.release();
  }

  if (glMajor == 2) {
    std::unique_ptr<GLES2Context> context(new GLES2Context);
    if (!context) {
      LOGE << "Could not create render context.";
      return nullptr;
    }
    if (context->initVG() != Status::OK) { return nullptr; }
    return context.release();
  }
  return nullptr;
}

} // namespace fui
