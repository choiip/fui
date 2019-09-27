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

VkCommandBuffer createAndBeginLocalCommandBuffer(VkDevice device, VkCommandPool commandPool);
void endCommandAndSubmitToQueue(VkCommandBuffer commandBuffer, VkQueue queue);

void setupImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout);

#ifdef __cplusplus
}
#endif
