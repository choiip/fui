#pragma once

#include "core/RenderContext.hpp"
#include "vulkan/vku.h"

namespace fui {

class VulkanContext : public RenderContext {
public:
  struct Resource {
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice gpu;
    VulkanDevice* device;
    VkQueue queue;
    FrameBuffers frameBuffer;
    VkCommandBuffer cmdBuffer;
#ifndef NDEBUG
    VkDebugReportCallbackEXT debugCallback;
#endif
  };

public:
  VulkanContext(NVGcontext* vg, Resource& resource);
  ~VulkanContext();

  VulkanDevice* device() { return _resource.device; }
  VkQueue queue() { return _resource.queue; }
  FrameBuffers* frameBuffer() { return &_resource.frameBuffer; }
  VkCommandBuffer cmdBuffer() { return _resource.cmdBuffer; }

private:
  Resource _resource;
};

} // namespace fui
