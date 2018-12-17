#include "core/ApplicationContext.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include "core/RenderWindow.hpp"
#include "core/Status.hpp"

#include <GLFW/glfw3.h>

namespace fui {

static void mainLoop(void* context) {
  auto appContext = static_cast<ApplicationContext*>(context);
  appContext->drawOnce();
  glfwPollEvents();
}

ApplicationContext::ApplicationContext()
: window(nullptr) {}

ApplicationContext::~ApplicationContext() = default;

void ApplicationContext::run(RenderWindow& window) {
  auto nativeWindow = static_cast<GLFWwindow*>(window.nativeWindow());
  this->window = &window;

  onEnter();

#if __EMSCRIPTEN__
  emscripten_set_main_loop_arg(mainLoop, this, 0, 1);
#else
  while (!glfwWindowShouldClose(nativeWindow)) {
    mainLoop(this);
  }
#endif
  onExit();
}

void ApplicationContext::drawOnce() { onDraw(); }

Status ApplicationContext::onEnter() { return Status::OK; }

void ApplicationContext::onDraw() {}

void ApplicationContext::onExit() {}

} // namespace fui
