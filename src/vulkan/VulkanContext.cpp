#include "VulkanContext.hpp"
#include "vku.h"
#include <vulkan/vulkan.h>
#define NANOVG_VULKAN_IMPLEMENTATION
#include "nanovg_vk.h"
#include "core/Log.hpp"
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
  int flag = NVG_ANTIALIAS | NVG_STENCIL_STROKES;
#ifdef NDEBUG
  flag |= NVG_DEBUG;
#endif
  _vg = nvgCreateVk(createInfo, flag);
  if (_vg == NULL) {
    LOGE << "Could not init nanovg (Vulkan).";
    return Status::UNKNOWN_ERROR;
  }

  return Status::OK;
}

std::string VulkanContext::versionLine() const {
  return "#version 400";
}

std::string VulkanContext::vertexMacro() const {
  return R"(
    #define VERT_IN in
    #define VERT_OUT out
  )";
}

std::string VulkanContext::fragmentMacro() const {
  return R"(
    #define FRAG_IN in
    #define FRAG_OUT out
  )";
}

} // namespace fui
