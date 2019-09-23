#include "GlfwVulkanProfile.hpp"
#include <memory>
#if defined(__APPLE__)
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_NONE
#else
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#include "core/Log.hpp"
#include "core/Status.hpp"
#include "vulkan/vku.hpp"
#include "vulkan/VulkanContext.hpp"

namespace fui {

GlfwVulkanProfile::GlfwVulkanProfile() = default;

GlfwVulkanProfile::~GlfwVulkanProfile() = default;

void GlfwVulkanProfile::prepare() const {
  if (!glfwVulkanSupported()) {
    LOGE << "GLFW failed to find the Vulkan loader.";
  }

  //// https://www.glfw.org/docs/latest/window_guide.html#window_hints
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

RenderContext* GlfwVulkanProfile::createContext(void* nativeWindow) const {
  GLFWwindow* window = (GLFWwindow*)nativeWindow;

  if (_instance == nullptr) {
    uint32_t extensionCount = 0;
    const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    std::vector<std::string> extensions;
    for (auto i=0; i<extensionCount; ++i) {
      extensions.push_back(requiredExtensions[i]);
    }

    std::vector<std::string> layers = {
#if !defined(NDEBUG)
      "VK_LAYER_LUNARG_standard_validation",
      // "VK_LAYER_LUNARG_core_validation",
      // "VK_LAYER_KHRONOS_validation",
      // "VK_LAYER_LUNARG_api_dump",
      // "VK_LAYER_LUNARG_object_tracker",
      // "VK_LAYER_LUNARG_screenshot",
#endif
    };
    auto non_const_this = const_cast<GlfwVulkanProfile*>(this);
    non_const_this->_instance = std::make_shared<vk::UniqueInstance>(vk::su::createInstance("", "fui", layers, extensions));
#if !defined(NDEBUG)
    non_const_this->_debugReportCallback = std::make_shared<vk::UniqueDebugReportCallbackEXT>(vk::su::createDebugReportCallback(*_instance));
#endif
  }

  VkSurfaceKHR surface;
  auto res = glfwCreateWindowSurface(_instance->get(), window, 0, &surface);
  if (VK_SUCCESS != res) {
    LOGE << "glfwCreateWindowSurface failed";
    return nullptr;
  }

  vk::Extent2D windowExtent;
  glfwGetWindowSize(window, (int*)(&windowExtent.width), (int*)(&windowExtent.height));

  std::unique_ptr<VulkanContext> context(new VulkanContext(_instance, surface, windowExtent, _debugReportCallback));
  if (!context) {
    LOGE << "Could not create render context.";
    return nullptr;
  }
  if (context->initVG() != Status::OK) {
    return nullptr;
  }

  return context.release();
}

} // namespace fui
