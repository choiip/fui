#include "GlfwGL3Profile.hpp"
#include <GL/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>
#include "core/Log.hpp"
#include "core/Status.hpp"
#include "GL/GL3Context.hpp"

namespace fui {

void GlfwGL3Profile::prepare() const {
  //// https://www.glfw.org/docs/latest/window_guide.html#window_hints
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#ifndef _WIN32 // don't require this on win32, and works with more cards
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
}

RenderContext* GlfwGL3Profile::createContext(void* nativeWindow) const {
  GLFWwindow* window = (GLFWwindow*)nativeWindow;
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOGE << "Could not initialize GLAD!";
    return nullptr;
  }
  glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM

  std::unique_ptr<GL3Context> context(new GL3Context);
  if (!context) {
    LOGE << "Could not create render context.";
    return nullptr;
  }
  if (context->initVG() != Status::OK) { return nullptr; }

  return context.release();
}

} // namespace fui
