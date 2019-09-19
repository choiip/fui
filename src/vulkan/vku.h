#pragma once

#include <vulkan/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VulkanDevice {
  VkPhysicalDevice gpu;
  VkPhysicalDeviceProperties gpuProperties;
  VkPhysicalDeviceMemoryProperties memoryProperties;

  VkQueueFamilyProperties* queueFamilyProperties;
  uint32_t queueFamilyPropertiesCount;

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

VkInstance createVkInstance(const char** extensions, uint32_t extensionCount, int enableDebugLayer);
VulkanDevice* createVulkanDevice(VkPhysicalDevice gpu);
void destroyVulkanDevice(VulkanDevice* device);

VkDebugReportCallbackEXT createDebugReport(VkInstance instance);
void destroyDebugReport(VkInstance instance, VkDebugReportCallbackEXT debugReportCallback);

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
