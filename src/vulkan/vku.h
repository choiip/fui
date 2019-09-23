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

typedef struct VulkanDevice {
  VkPhysicalDevice gpu;
  VkPhysicalDeviceMemoryProperties memoryProperties;

  uint32_t graphicsQueueFamilyIndex;

  VkDevice device;
  VkCommandPool commandPool;
} VulkanDevice;

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
  VkSemaphore present_complete_semaphore;
  VkSemaphore render_complete_semaphore;

} FrameBuffers;

VulkanDevice* createVulkanDevice(VkPhysicalDevice gpu);
void destroyVulkanDevice(VulkanDevice* device);

VkCommandPool createCmdPool(VulkanDevice* device);
VkCommandBuffer createCmdBuffer(VkDevice device, VkCommandPool cmd_pool);
VkCommandBuffer createAndBeginLocalCommandBuffer(VkDevice device, VkCommandPool commandPool);
void endCommandAndSubmitToQueue(VkCommandBuffer commandBuffer, VkQueue queue);

DepthBuffer createDepthBuffer(const VulkanDevice* device, int width, int height, VkFormat format);

void setupImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout);

VkRenderPass createRenderPass(VkDevice device, VkFormat color_format, VkFormat depth_format);

SwapchainBuffers createSwapchainBuffers(VkDevice device, VkFormat format, VkCommandBuffer cmdbuffer, VkImage image);

FrameBuffers createFrameBuffers(const VulkanDevice* device, VkSurfaceKHR surface, VkQueue queue,
                                VkExtent2D windowExtent, VkSwapchainKHR oldSwapchain);

void destroyFrameBuffers(const VulkanDevice* device, FrameBuffers* buffer);

#ifdef __cplusplus
}
#endif
