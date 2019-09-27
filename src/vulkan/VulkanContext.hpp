#pragma once

#include <memory>
#include "core/RenderContext.hpp"
#include <vulkan/vulkan.hpp>

namespace fui {

enum class Status;

class VulkanContext : public RenderContext {
public:
  VulkanContext(const std::shared_ptr<vk::UniqueInstance>& instance,
                const std::shared_ptr<vk::UniqueDebugReportCallbackEXT>& debugReportCallback);
  ~VulkanContext();

  Status initSwapchain(const vk::SurfaceKHR& surface, const vk::Extent2D& windowExtent);
  Status initFramebuffer(const vk::Extent2D& extent);
  Status initVG();

  virtual auto setViewport(int x, int y, int width, int height) -> decltype(this) override;
  virtual auto preDraw(const Recti& renderArea, const Color* clearColor = nullptr, const float* clearDepth = nullptr,
                       const int* clearStencil = nullptr) -> decltype(this) override;
  virtual auto postDraw() -> decltype(this) override;

protected:
  Status rebuildSwapchain();

  virtual std::string versionLine() const override;
  virtual std::string vertexMacro() const override;
  virtual std::string fragmentMacro() const override;

private:
  struct Private;
  std::unique_ptr<Private> _p;
};

} // namespace fui
