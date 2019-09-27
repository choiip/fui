#include "vulkan/vku.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

VkCommandBuffer createCommandBuffer(VkDevice device, VkCommandPool commandPool) {
  VkCommandBufferAllocateInfo info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
  info.commandPool = commandPool;
  info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  info.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &info, &commandBuffer));
  return commandBuffer;
}

VkCommandBuffer createAndBeginLocalCommandBuffer(VkDevice device, VkCommandPool commandPool) {
  VkCommandBuffer commandBuffer = createCommandBuffer(device, commandPool);

  const VkCommandBufferBeginInfo commandBufferBeginInfo = {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
  };
  vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

  return commandBuffer;
}

void endCommandAndSubmitToQueue(VkCommandBuffer commandBuffer, VkQueue queue) {
  vkEndCommandBuffer(commandBuffer);
  VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
}

void setupImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout) {
  // reference: https://github.com/DiligentGraphics/DiligentCore/blob/master/Graphics/GraphicsEngineVulkan/src/VulkanUtilities/VulkanCommandBuffer.cpp
  VkAccessFlags sourceAccessMask = 0;
  switch (oldImageLayout)
  {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      sourceAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      break;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
      sourceAccessMask = VK_ACCESS_HOST_WRITE_BIT;
      break;
    case VK_IMAGE_LAYOUT_GENERAL:     // sourceAccessMask is empty
      sourceAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
      break;
    case VK_IMAGE_LAYOUT_UNDEFINED:
      break;
    default:
      assert(0);
      break;
  }

  VkPipelineStageFlags sourceStage;
  switch (oldImageLayout)
  {
    case VK_IMAGE_LAYOUT_GENERAL:
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
      sourceStage = VK_PIPELINE_STAGE_HOST_BIT;
      break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      break;
    case VK_IMAGE_LAYOUT_UNDEFINED:
      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      break;
    default:
      assert(0);
      break;
  }

  VkAccessFlags destinationAccessMask;
  switch (newImageLayout)
  {
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      destinationAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      destinationAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      break;
    case VK_IMAGE_LAYOUT_GENERAL:   // empty destinationAccessMask
      break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      destinationAccessMask = VK_ACCESS_SHADER_READ_BIT;
      break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      destinationAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      destinationAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      break;
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
      destinationAccessMask = VK_ACCESS_MEMORY_READ_BIT;
      break;
    default:
      assert(0);
      break;
  }

  VkPipelineStageFlags destinationStage;
  switch (newImageLayout)
  {
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
      break;
    case VK_IMAGE_LAYOUT_GENERAL:
      destinationStage = VK_PIPELINE_STAGE_HOST_BIT;
      break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      break;
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
      destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      break;      
    default:
      assert(0);
      break;
  }

  VkImageAspectFlags aspectMask;
  if (newImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
  {
    aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT)
    {
      aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
  }
  else
  {
    aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  }

  VkImageSubresourceRange imageSubresourceRange = { aspectMask, 0, 1, 0, 1 };
  VkImageMemoryBarrier imageMemoryBarrier = { 
                                              VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                                              0,
                                              sourceAccessMask, 
                                              destinationAccessMask, 
                                              oldImageLayout, 
                                              newImageLayout, 
                                              VK_QUEUE_FAMILY_IGNORED, 
                                              VK_QUEUE_FAMILY_IGNORED,
                                              image,
                                              imageSubresourceRange 
                                            };
  vkCmdPipelineBarrier(cmdBuffer, sourceStage, destinationStage, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier);
}
