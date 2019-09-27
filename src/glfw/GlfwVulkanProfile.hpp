#pragma once

#include <memory>
#include "core/GraphicsProfile.hpp"
#include <vulkan/vulkan.hpp>

namespace fui {

class GlfwVulkanProfile : public GraphicsProfile {
public:
  GlfwVulkanProfile();

  virtual ~GlfwVulkanProfile();

  virtual GraphicsAPI api() const override { return GraphicsAPI::VULKAN; }

  virtual void prepare() const override;

  virtual RenderContext* createContext(void* nativeWindow) const override;

private:
  std::shared_ptr<vk::UniqueInstance>               _instance;
  std::shared_ptr<vk::UniqueDebugReportCallbackEXT> _debugReportCallback;
};

} // namespace fui
