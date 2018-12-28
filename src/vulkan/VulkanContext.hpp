#pragma once

#include "core/RenderContext.hpp"
#include "vulkan/vku.h"

namespace fui {

enum class Status;

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
  explicit VulkanContext(Resource& resource);
  ~VulkanContext();

  Status initVG();

  VulkanDevice* device() const { return _resource.device; }
  VkQueue queue() const { return _resource.queue; }
  FrameBuffers* frameBuffer() { return &_resource.frameBuffer; }
  VkCommandBuffer cmdBuffer() const { return _resource.cmdBuffer; }

private:
  Resource _resource;
};

} // namespace fui
