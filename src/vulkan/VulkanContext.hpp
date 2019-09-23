#pragma once

#include <memory>
#include "core/RenderContext.hpp"
#include <vulkan/vulkan.hpp>

namespace fui {

enum class Status;

class VulkanContext : public RenderContext {
public:
  VulkanContext(const std::shared_ptr<vk::UniqueInstance>& instance, 
                vk::SurfaceKHR surface, 
                const vk::Extent2D& windowExtent,
                const std::shared_ptr<vk::UniqueDebugReportCallbackEXT>& debugReportCallback);
  ~VulkanContext();

  Status initVG();

  virtual auto setViewport(int x, int y, int width, int height) -> decltype(this) override;
  virtual auto preDraw(const Color* clearColor = nullptr, const float* clearDepth = nullptr, const int* clearStencil = nullptr) -> decltype(this) override;
  virtual auto postDraw() -> decltype(this) override;

protected:
  virtual std::string versionLine() const override;
  virtual std::string vertexMacro() const override;
  virtual std::string fragmentMacro() const override;

private:
  struct Private;
  std::unique_ptr<Private> _p;
};

} // namespace fui
