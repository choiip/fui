#include "core/GlfwWindowManager.hpp"
#include <memory>
#include <GLFW/glfw3.h>
#include "core/GlfwCursor.hpp"
#include "core/GlfwRenderWindow.hpp"
#include "core/Log.hpp"
#include "core/RenderContext.hpp"
#include "event/EventEnum.hpp"
#include "GL/GlfwGL3Profile.hpp"
#include "GL/GlfwGLES2Profile.hpp"
#ifdef FUI_ENABLE_VULKAN
#include "vulkan/GlfwVulkanProfile.hpp"
#endif

namespace fui {

static void errorCallback(int error, const char* description) {
  LOGE << "GlfwWindowManager error: " << description << '\n';
}

static void setupCallbacks(GLFWwindow* glfwWindow, RenderWindow* renderWindow) {
  glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* window) {
    const auto& windows = GlfwWindowManager::instance().getWindows();
    auto targetWindow = windows.find(window);
    if (targetWindow != windows.end()) {
      delete targetWindow->second;
      const_cast<std::unordered_map<GLFWwindow*, GlfwRenderWindow*>&>(windows).erase(targetWindow);
    }
  });
  glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height) {
    const auto& windows = GlfwWindowManager::instance().getWindows();
    auto targetWindow = windows.find(window);
    if (targetWindow != windows.end()) {
      targetWindow->second->onResizeEvent(width, height);
    }
  });
  glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    const auto& windows = GlfwWindowManager::instance().getWindows();
    auto targetWindow = windows.find(window);
    if (targetWindow != windows.end()) {
      targetWindow->second->onKeyEvent((Key)key, (ButtonAction)action, (Modifier)mods);
    }
  });
  glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods) {
    constexpr MouseButton mouseButton[]{
        MouseButton::LEFT, MouseButton::RIGHT, MouseButton::MIDDLE, MouseButton::X1, MouseButton::X2,
    };
    if (button < 0 || button > 4)
      return;
    const auto& windows = GlfwWindowManager::instance().getWindows();
    auto targetWindow = windows.find(window);
    if (targetWindow != windows.end()) {
      targetWindow->second->onMouseButtonEvent(mouseButton[button], (ButtonAction)action, (Modifier)mods);
    }
  });
  glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xpos, double ypos) {
    const auto& windows = GlfwWindowManager::instance().getWindows();
    auto targetWindow = windows.find(window);
    if (targetWindow != windows.end()) {
      targetWindow->second->onMouseMoveEvent((int)xpos, (int)ypos);
    }
  });
}

GlfwWindowManager::GlfwWindowManager() {
  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) {
    LOGE << "Failed to init GLFW.";
  }
}

GlfwWindowManager::~GlfwWindowManager() { glfwTerminate(); }

RenderWindow* GlfwWindowManager::createWindow(int width, int height, const GraphicsProfile& graphicsProfile) {
  graphicsProfile.prepare();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  auto glfwWindow = glfwCreateWindow(width, height, "Title", NULL, NULL);
  if (!glfwWindow) {
    LOGE << "Could not create GLFW window!";
    return nullptr;
  }

  auto renderContext = graphicsProfile.createContext(glfwWindow);
  if (!renderContext) {
    glfwDestroyWindow(glfwWindow);
    LOGE << "Could not create render context!";
    return nullptr;
  }

  std::unique_ptr<GlfwRenderWindow> renderWindow(new GlfwRenderWindow(glfwWindow, renderContext));
  if (!renderWindow) {
    delete renderContext;
    glfwDestroyWindow(glfwWindow);
    LOGE << "Could not create render context!";
    return nullptr;
  }

  // set callbacks
  setupCallbacks(glfwWindow, renderWindow.get());

  _windows.insert(std::make_pair(glfwWindow, renderWindow.get()));
  return renderWindow.release();
}

GraphicsProfile* GlfwWindowManager::createGraphicsProfile(GraphicsAPI api, int major, int minor) {
#ifndef __EMSCRIPTEN__
  if (api == GraphicsAPI::OPENGL)
    return new GlfwGL3Profile();
#endif
  if (api == GraphicsAPI::OPENGL_ES)
    return new GlfwGLES2Profile();
#ifdef FUI_ENABLE_VULKAN
  if (api == GraphicsAPI::VULKAN)
    return new GlfwVulkanProfile();
#endif
  return nullptr;
}

void GlfwWindowManager::pollEvent() { glfwPollEvents(); }

bool GlfwWindowManager::shouldQuit() { return not hasRunableWindow(); }

bool GlfwWindowManager::hasRunableWindow() const {
  for (auto&& w : _windows) {
    if (!glfwWindowShouldClose(w.first)) {
      return true;
    }
  }
  return false;
}

auto GlfwWindowManager::getWindows() const -> const decltype(_windows)& { return _windows; }

} // namespace fui
