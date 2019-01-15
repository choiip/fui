#include "core/ApplicationContext.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include "core/AbstractWindowManager.hpp"
#include "core/Status.hpp"

namespace fui {

struct MainLoopArgument {
  ApplicationContext* appContext;
  AbstractWindowManager* windowManager;
};

static void mainLoop(void* argu) {
  auto mainLoopArgu = static_cast<MainLoopArgument*>(argu);
  mainLoopArgu->appContext->drawOnce();
  mainLoopArgu->windowManager->pollEvent();
}

ApplicationContext::ApplicationContext() {}

ApplicationContext::~ApplicationContext() {}

void ApplicationContext::run(AbstractWindowManager& windowManager) {
  MainLoopArgument argu = { this, &windowManager };
  onEnter();

#if __EMSCRIPTEN__
  emscripten_set_main_loop_arg(mainLoop, &argu, 0, 1);
#else
  while (!windowManager.shouldQuit()) {
    mainLoop(&argu);
  }
#endif
  onExit();
}

void ApplicationContext::drawOnce() { onDraw(); }

Status ApplicationContext::onEnter() { return Status::OK; }

void ApplicationContext::onDraw() {}

void ApplicationContext::onExit() {}

} // namespace fui
