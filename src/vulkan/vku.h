#pragma once

#include <vulkan/vulkan.h>

#define VK_CHECK_RESULT(f)                                                                                           \
{                                                                                                                    \
  VkResult res = (f);                                                                                                \
  if (res != VK_SUCCESS) {                                                                                           \
    assert(res == VK_SUCCESS);                                                                                       \
  }                                                                                                                  \
}

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SwapchainBuffers {
  VkImage image;
  VkImageView view;
} SwapchainBuffers;

typedef struct DepthBuffer {
  VkFormat format;

  VkImage image;
  VkDeviceMemory mem;
  VkImageView view;
} DepthBuffer;

typedef struct FrameBuffers {
  VkSwapchainKHR swap_chain;
  SwapchainBuffers* swap_chain_buffers;
  uint32_t swapchain_image_count;
  VkFramebuffer* framebuffers;

  uint32_t current_buffer;

  VkExtent2D buffer_size;

  VkRenderPass render_pass;

  VkFormat format;
  DepthBuffer depth;
} FrameBuffers;

VkCommandBuffer createCmdBuffer(VkDevice device, VkCommandPool cmd_pool);
VkCommandBuffer createAndBeginLocalCommandBuffer(VkDevice device, VkCommandPool commandPool);
void endCommandAndSubmitToQueue(VkCommandBuffer commandBuffer, VkQueue queue);

DepthBuffer createDepthBuffer(VkPhysicalDevice gpu, VkDevice device, int width, int height, VkFormat format);

void setupImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout);

VkRenderPass createRenderPass(VkDevice device, VkFormat color_format, VkFormat depth_format);

SwapchainBuffers createSwapchainBuffers(VkDevice device, VkFormat format, VkCommandBuffer cmdbuffer, VkImage image);

FrameBuffers createFrameBuffers(VkPhysicalDevice gpu, VkDevice device, VkCommandPool commandPool, VkSurfaceKHR surface, VkQueue queue,
                                VkExtent2D windowExtent, VkSwapchainKHR oldSwapchain);

void destroyFrameBuffers(VkDevice device, FrameBuffers* buffer);

#ifdef __cplusplus
}
#endif
