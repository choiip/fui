#pragma once

#include "core/GraphicsProfile.hpp"

namespace fui {

class GlfwVulkanProfile : public GraphicsProfile {
public:
  virtual ~GlfwVulkanProfile();

  virtual GraphicsAPI api() const override { return GraphicsAPI::VULKAN; }

  virtual void prepare() const override;

  virtual RenderContext* createContext(void* nativeWindow) const override;

private:
};

} // namespace fui
