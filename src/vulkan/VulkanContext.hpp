#pragma once

#include <memory>
#include "core/RenderContext.hpp"
#include "vulkan/vku.h"

namespace fui {

enum class Status;

class VulkanContext : public RenderContext {
public:
  struct Resource {
    std::shared_ptr<VkInstance> instance;
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

  virtual auto setViewport(int x, int y, int width, int height) -> decltype(this) override;
  virtual auto preDraw(const Color* clearColor = nullptr, const float* clearDepth = nullptr, const int* clearStencil = nullptr) -> decltype(this) override;
  virtual auto postDraw() -> decltype(this) override;

protected:
  virtual std::string versionLine() const override;
  virtual std::string vertexMacro() const override;
  virtual std::string fragmentMacro() const override;

private:
  Resource _resource;
};

} // namespace fui
