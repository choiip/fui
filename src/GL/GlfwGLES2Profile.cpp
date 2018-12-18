#include "GlfwGLES2Profile.hpp"
#include <GL/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

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
  if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << ("Could not initialize GLAD!\n");
    return nullptr;
  }
  glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM

  int glMajor = 0;
#ifdef __EMSCRIPTEN__
  glMajor = 2;
#else
  glfwGetVersion(&glMajor, NULL, NULL);
#endif

  if (glMajor == 3) {
    std::unique_ptr<GLES3Context> context(new GLES3Context);
    if (!context) {
      std::cerr << ("Could not create render context.\n");
      return nullptr;
    }
    if (context->initVG() != Status::OK) {
      return nullptr;
    }
    return context.release();    
  }

  if (glMajor == 2) {
    std::unique_ptr<GLES2Context> context(new GLES2Context);
    if (!context) {
      std::cerr << ("Could not create render context.\n");
      return nullptr;
    }
    if (context->initVG() != Status::OK) {
      return nullptr;
    }
    return context.release();    
  }
  return nullptr;
}

} // namespace fui
