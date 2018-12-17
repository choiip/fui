#include "VulkanContext.hpp"
#include <iostream>
#include "vku.h"
#include <vulkan/vulkan.h>
#define NANOVG_VULKAN_IMPLEMENTATION
#include "nanovg_vk.h"
#include "core/Status.hpp"

namespace fui {

VulkanContext::VulkanContext(Resource& resource)
: _resource(resource) {}

VulkanContext::~VulkanContext() {
  nvgDeleteVk(vg());

  destroyFrameBuffers(_resource.device, &_resource.frameBuffer);

  destroyVulkanDevice(_resource.device);

#ifndef NDEBUG
  DestroyDebugReport(_resource.instance, _resource.debugCallback);
#endif
  vkDestroySurfaceKHR(_resource.instance, _resource.surface, NULL);
  vkDestroyInstance(_resource.instance, NULL);
}

Status VulkanContext::initVG() {
  if (_vg != nullptr) {
    nvgDeleteVk(_vg);
  }
  VKNVGCreateInfo createInfo = {0};
  createInfo.device = _resource.device->device;
  createInfo.gpu = _resource.gpu;
  createInfo.renderpass = _resource.frameBuffer.render_pass;
  createInfo.cmdBuffer = _resource.cmdBuffer;

  _vg = nvgCreateVk(createInfo, NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
  if (_vg == NULL) {
    std::cerr << ("Could not init nanovg (Vulkan).\n");
    return Status::UNKNOWN_ERROR;
  }

  return Status::OK;
}

} // namespace fui
