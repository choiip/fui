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
#include "vulkan/vku.h"
#include "vulkan/VulkanContext.hpp"

namespace fui {

GlfwVulkanProfile::~GlfwVulkanProfile() {}

void GlfwVulkanProfile::prepare() const {
  if (!glfwVulkanSupported()) {
    LOGE << "GLFW failed to find the Vulkan loader.";
  }

  //// https://www.glfw.org/docs/latest/window_guide.html#window_hints
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

RenderContext* GlfwVulkanProfile::createContext(void* nativeWindow) const {
  GLFWwindow* window = (GLFWwindow*)nativeWindow;

  VulkanContext::Resource resource;
  uint32_t extensionCount = 0;
  const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

#ifdef NDEBUG
  resource.instance = createVkInstance(extensions, extensionCount, false);
#else
  resource.instance = createVkInstance(extensions, extensionCount, true);
  resource.debugCallback = CreateDebugReport(resource.instance);
#endif

  VkResult res;

  uint32_t gpu_count = 1;
  res = vkEnumeratePhysicalDevices(resource.instance, &gpu_count, &resource.gpu);
  if (res != VK_SUCCESS && res != VK_INCOMPLETE) {
    LOGE << "vkEnumeratePhysicalDevices failed " << res;
    return nullptr;
  }
  auto device = resource.device = createVulkanDevice(resource.gpu);

  int winWidth, winHeight;
  glfwGetWindowSize(window, &winWidth, &winHeight);

  vkGetDeviceQueue(device->device, device->graphicsQueueFamilyIndex, 0, &resource.queue);

  res = glfwCreateWindowSurface(resource.instance, window, 0, &resource.surface);
  if (VK_SUCCESS != res) {
    LOGE << "glfwCreateWindowSurface failed";
    return nullptr;
  }

  resource.frameBuffer = createFrameBuffers(device, resource.surface, resource.queue, winWidth, winHeight, 0);
  resource.cmdBuffer = createCmdBuffer(device->device, device->commandPool);

  std::unique_ptr<VulkanContext> context(new VulkanContext(resource));
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