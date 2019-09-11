#include "VulkanContext.hpp"
#include "vku.h"
#include <vulkan/vulkan.h>
#define NANOVG_VULKAN_IMPLEMENTATION
#include "nanovg_vk.h"
#include "core/Log.hpp"
#include "core/MathDef.hpp"
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

auto VulkanContext::setViewport(int x, int y, int width, int height) -> decltype(this) {
  VkViewport viewport = {
    (float)x, (float)y,
    (float)width, (float)height,
    0.f, 1.f    
  };
  vkCmdSetViewport(_resource.cmdBuffer, 0, 1, &viewport);
  return this;
}

auto VulkanContext::preDraw(const Color* clearColor, const float* clearDepth, const int* clearStencil) -> decltype(this) {
  auto&& fb = _resource.frameBuffer;
  VkResult res;

  if (clearColor != nullptr) {
  }
  if (clearDepth != nullptr) {
  }
  if (clearStencil != nullptr) {
  }
  VkClearValue clear_values[2];
  clear_values[0].color.float32[0] = 0.3f;
  clear_values[0].color.float32[1] = 0.3f;
  clear_values[0].color.float32[2] = 0.32f;
  clear_values[0].color.float32[3] = 1.0f;
  clear_values[1].depthStencil.depth = 1.0f;
  clear_values[1].depthStencil.stencil = 0;

  VkRenderPassBeginInfo rp_begin;
  rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rp_begin.pNext = NULL;
  rp_begin.renderPass = fb.render_pass;
  rp_begin.framebuffer = fb.framebuffers[fb.current_buffer];
  rp_begin.renderArea.offset.x = 0;
  rp_begin.renderArea.offset.y = 0;
  rp_begin.renderArea.extent.width = fb.buffer_size.width;
  rp_begin.renderArea.extent.height = fb.buffer_size.height;
  rp_begin.clearValueCount = 2;
  rp_begin.pClearValues = clear_values;
  
  // Get the index of the next available swapchain image:
  res = vkAcquireNextImageKHR(_resource.device->device, fb.swap_chain, UINT64_MAX, fb.present_complete_semaphore, 0,
                              &fb.current_buffer);
  assert(res == VK_SUCCESS);

  const VkCommandBufferBeginInfo cmd_buf_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  res = vkBeginCommandBuffer(_resource.cmdBuffer, &cmd_buf_info);
  assert(res == VK_SUCCESS);  
  
  
  vkCmdBeginRenderPass(_resource.cmdBuffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

  // VkViewport viewport;
  // viewport.width = fb.buffer_size.width;
  // viewport.height = fb.buffer_size.height;
  // viewport.minDepth = (float)0.0f;
  // viewport.maxDepth = (float)1.0f;
  // viewport.x = rp_begin.renderArea.offset.x;
  // viewport.y = rp_begin.renderArea.offset.y;
  // vkCmdSetViewport(_resource.cmdBuffer, 0, 1, &viewport);
  
  VkRect2D scissor = rp_begin.renderArea;
  vkCmdSetScissor(_resource.cmdBuffer, 0, 1, &scissor);
  return this;
}

auto VulkanContext::postDraw() -> decltype(this) {
  auto&& fb = _resource.frameBuffer;
  VkResult res;

  vkCmdEndRenderPass(_resource.cmdBuffer);

  vkEndCommandBuffer(_resource.cmdBuffer);

  VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submit_info.pNext = NULL;
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = &fb.present_complete_semaphore;
  submit_info.pWaitDstStageMask = &pipe_stage_flags;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &_resource.cmdBuffer;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = &fb.render_complete_semaphore;

  /* Queue the command buffer for execution */
  res = vkQueueSubmit(_resource.queue, 1, &submit_info, 0);
  assert(res == VK_SUCCESS);

  /* Now present the image in the window */

  VkPresentInfoKHR present = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
  present.pNext = NULL;
  present.swapchainCount = 1;
  present.pSwapchains = &fb.swap_chain;
  present.pImageIndices = &fb.current_buffer;
  present.waitSemaphoreCount = 1;
  present.pWaitSemaphores = &fb.render_complete_semaphore;

  res = vkQueuePresentKHR(_resource.queue, &present);
  assert(res == VK_SUCCESS);

  res = vkQueueWaitIdle(_resource.queue);
  assert(res == VK_SUCCESS);

  return this;
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
