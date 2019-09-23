#include "VulkanContext.hpp"
#include "vku.h"
#include <vulkan/vulkan.h>
#define NANOVG_VULKAN_IMPLEMENTATION
#include "nanovg_vk.h"
#include "core/Log.hpp"
#include "core/MathDef.hpp"
#include "core/Status.hpp"

namespace fui {

struct VulkanContext::Private {
  std::shared_ptr<vk::UniqueInstance> instance;
  vk::SurfaceKHR surface;
  VkPhysicalDevice gpu;
  VulkanDevice* device;
  VkQueue queue;
  FrameBuffers frameBuffer;
  VkCommandBuffer cmdBuffer;
  std::shared_ptr<vk::UniqueDebugReportCallbackEXT> debugCallback;
};

VulkanContext::VulkanContext(const std::shared_ptr<vk::UniqueInstance>& instance, 
                             vk::SurfaceKHR surface, 
                             const vk::Extent2D& windowExtent,
                             const std::shared_ptr<vk::UniqueDebugReportCallbackEXT>& debugReportCallback)
{
  _p.reset(new Private);
  _p->instance = instance;
  _p->surface = surface;
  _p->debugCallback = debugReportCallback;
  uint32_t gpu_count = 1;
  VkResult res = vkEnumeratePhysicalDevices(instance->get(), &gpu_count, &_p->gpu);
  if (res != VK_SUCCESS && res != VK_INCOMPLETE) {
    LOGE << "vkEnumeratePhysicalDevices failed " << res;
  }
  auto device = _p->device = createVulkanDevice(_p->gpu);

  vkGetDeviceQueue(device->device, device->graphicsQueueFamilyIndex, 0, &_p->queue);

  _p->frameBuffer = createFrameBuffers(device, surface, _p->queue, windowExtent, 0);
  _p->cmdBuffer = createCmdBuffer(device->device, device->commandPool);
}

VulkanContext::~VulkanContext() {
  nvgDeleteVk(vg());

  destroyFrameBuffers(_p->device, &_p->frameBuffer);

  destroyVulkanDevice(_p->device);

  vkDestroySurfaceKHR(_p->instance->get(), _p->surface, NULL);
}

Status VulkanContext::initVG() {
  if (_vg != nullptr) {
    nvgDeleteVk(_vg);
  }
  VKNVGCreateInfo createInfo = {0};
  createInfo.gpu = _p->gpu;
  createInfo.device = _p->device->device;
  createInfo.commandPool = _p->device->commandPool;
  createInfo.queue = _p->queue;
  createInfo.renderpass = _p->frameBuffer.render_pass;
  createInfo.cmdBuffer = _p->cmdBuffer;
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
  vkCmdSetViewport(_p->cmdBuffer, 0, 1, &viewport);
  return this;
}

auto VulkanContext::preDraw(const Color* clearColor, const float* clearDepth, const int* clearStencil) -> decltype(this) {
  auto&& fb = _p->frameBuffer;
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
  res = vkAcquireNextImageKHR(_p->device->device, fb.swap_chain, UINT64_MAX, fb.present_complete_semaphore, 0,
                              &fb.current_buffer);
  assert(res == VK_SUCCESS);

  const VkCommandBufferBeginInfo cmd_buf_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  res = vkBeginCommandBuffer(_p->cmdBuffer, &cmd_buf_info);
  assert(res == VK_SUCCESS);  
  
  
  vkCmdBeginRenderPass(_p->cmdBuffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

  // VkViewport viewport;
  // viewport.width = fb.buffer_size.width;
  // viewport.height = fb.buffer_size.height;
  // viewport.minDepth = (float)0.0f;
  // viewport.maxDepth = (float)1.0f;
  // viewport.x = rp_begin.renderArea.offset.x;
  // viewport.y = rp_begin.renderArea.offset.y;
  // vkCmdSetViewport(_p->cmdBuffer, 0, 1, &viewport);
  
  VkRect2D scissor = rp_begin.renderArea;
  vkCmdSetScissor(_p->cmdBuffer, 0, 1, &scissor);
  return this;
}

auto VulkanContext::postDraw() -> decltype(this) {
  auto&& fb = _p->frameBuffer;
  VkResult res;

  vkCmdEndRenderPass(_p->cmdBuffer);

  vkEndCommandBuffer(_p->cmdBuffer);

  VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submit_info.pNext = NULL;
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = &fb.present_complete_semaphore;
  submit_info.pWaitDstStageMask = &pipe_stage_flags;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &_p->cmdBuffer;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = &fb.render_complete_semaphore;

  /* Queue the command buffer for execution */
  res = vkQueueSubmit(_p->queue, 1, &submit_info, 0);
  assert(res == VK_SUCCESS);

  /* Now present the image in the window */

  VkPresentInfoKHR present = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
  present.pNext = NULL;
  present.swapchainCount = 1;
  present.pSwapchains = &fb.swap_chain;
  present.pImageIndices = &fb.current_buffer;
  present.waitSemaphoreCount = 1;
  present.pWaitSemaphores = &fb.render_complete_semaphore;

  res = vkQueuePresentKHR(_p->queue, &present);
  assert(res == VK_SUCCESS);

  res = vkQueueWaitIdle(_p->queue);
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
