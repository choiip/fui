#include "vulkan/vku.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

VulkanDevice* createVulkanDevice(VkPhysicalDevice gpu) {
  VulkanDevice* device = (VulkanDevice*)malloc(sizeof(VulkanDevice));
  memset(device, 0, sizeof(VulkanDevice));

  device->gpu = gpu;
  vkGetPhysicalDeviceMemoryProperties(gpu, &device->memoryProperties);

  VkQueueFamilyProperties* queueFamilyProperties;
  uint32_t queueFamilyPropertiesCount;

  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyPropertiesCount, NULL);
  assert(queueFamilyPropertiesCount >= 1);

  queueFamilyProperties =
      (VkQueueFamilyProperties*)malloc(queueFamilyPropertiesCount * sizeof(VkQueueFamilyProperties));

  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyPropertiesCount, queueFamilyProperties);
  assert(queueFamilyPropertiesCount >= 1);

  device->graphicsQueueFamilyIndex = UINT32_MAX;
  for (uint32_t i = 0; i < queueFamilyPropertiesCount; ++i) {
    if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
      device->graphicsQueueFamilyIndex = i;
    }
  }
  free(queueFamilyProperties);

  float queuePriorities[1] = {0.0};
  VkDeviceQueueCreateInfo queue_info = {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
  queue_info.queueCount = 1;
  queue_info.pQueuePriorities = queuePriorities;
  queue_info.queueFamilyIndex = device->graphicsQueueFamilyIndex;

  const char* deviceExtensions[] = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };
  VkDeviceCreateInfo deviceInfo = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
  deviceInfo.queueCreateInfoCount = 1;
  deviceInfo.pQueueCreateInfos = &queue_info;
  deviceInfo.enabledExtensionCount = sizeof(deviceExtensions) / sizeof(deviceExtensions[0]);
  deviceInfo.ppEnabledExtensionNames = deviceExtensions;
  VK_CHECK_RESULT(vkCreateDevice(gpu, &deviceInfo, NULL, &device->device));
  
  return device;
}
void destroyVulkanDevice(VulkanDevice* device) {
  if (device->device) {
    vkDestroyDevice(device->device, 0);
  }
  free(device);
}

VkCommandBuffer createCmdBuffer(VkDevice device, VkCommandPool cmd_pool) {
  VkCommandBufferAllocateInfo cmd = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
  cmd.commandPool = cmd_pool;
  cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cmd.commandBufferCount = 1;

  VkCommandBuffer cmd_buffer;
  VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &cmd, &cmd_buffer));
  return cmd_buffer;
}

VkCommandBuffer createAndBeginLocalCommandBuffer(VkDevice device, VkCommandPool commandPool) {
  VkCommandBuffer commandBuffer = createCmdBuffer(device, commandPool);

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

VkBool32 memory_type_from_properties(VkPhysicalDeviceMemoryProperties memoryProps, uint32_t typeBits,
                                     VkFlags requirements_mask, uint32_t* typeIndex) {
  // Search memtypes to find first index with those properties
  for (uint32_t i = 0; i < memoryProps.memoryTypeCount; i++) {
    if ((typeBits & 1) == 1) {
      // Type is available, does it match user properties?
      if ((memoryProps.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
        *typeIndex = i;
        return VK_TRUE;
      }
    }
    typeBits >>= 1;
  }
  // No memory types matched, return failure
  return VK_FALSE;
}
DepthBuffer createDepthBuffer(const VulkanDevice* device, int width, int height, VkFormat format) {
  DepthBuffer depth;
  depth.format = format;

  VkFormatProperties fprops;
  vkGetPhysicalDeviceFormatProperties(device->gpu, format, &fprops);
  VkImageTiling image_tilling;
  if (fprops.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
    image_tilling = VK_IMAGE_TILING_LINEAR;
  } else if (fprops.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
    image_tilling = VK_IMAGE_TILING_OPTIMAL;
  } else {
    /* Try other depth formats? */
    printf("depth_format %d Unsupported.\n", format);
    exit(-1);
  }

  VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  image_info.imageType = VK_IMAGE_TYPE_2D;
  image_info.format = format;
  image_info.tiling = image_tilling;
  image_info.extent.width = width;
  image_info.extent.height = height;
  image_info.extent.depth = 1;
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  image_info.queueFamilyIndexCount = 0;
  image_info.pQueueFamilyIndices = NULL;
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  VkMemoryAllocateInfo mem_alloc = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};

  VkImageViewCreateInfo view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  view_info.format = format;
  view_info.components.r = VK_COMPONENT_SWIZZLE_R;
  view_info.components.g = VK_COMPONENT_SWIZZLE_G;
  view_info.components.b = VK_COMPONENT_SWIZZLE_B;
  view_info.components.a = VK_COMPONENT_SWIZZLE_A;
  view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  view_info.subresourceRange.baseMipLevel = 0;
  view_info.subresourceRange.levelCount = 1;
  view_info.subresourceRange.baseArrayLayer = 0;
  view_info.subresourceRange.layerCount = 1;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;

  if (format == VK_FORMAT_D16_UNORM_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT ||
      format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
    view_info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
  }

  VkMemoryRequirements mem_reqs;

  /* Create image */
  VK_CHECK_RESULT(vkCreateImage(device->device, &image_info, NULL, &depth.image));

  vkGetImageMemoryRequirements(device->device, depth.image, &mem_reqs);

  mem_alloc.allocationSize = mem_reqs.size;
  /* Use the memory properties to determine the type of memory required */

  VkBool32 pass = memory_type_from_properties(device->memoryProperties, mem_reqs.memoryTypeBits,
                                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);
  assert(pass);

  /* Allocate memory */
  VK_CHECK_RESULT(vkAllocateMemory(device->device, &mem_alloc, NULL, &depth.mem));

  /* Bind memory */
  VK_CHECK_RESULT(vkBindImageMemory(device->device, depth.image, depth.mem, 0));

  /* Create image view */
  view_info.image = depth.image;
  VK_CHECK_RESULT(vkCreateImageView(device->device, &view_info, NULL, &depth.view));

  return depth;
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

SwapchainBuffers createSwapchainBuffers(VkDevice device, VkFormat format, VkCommandBuffer cmdbuffer, VkImage image) {
  setupImageLayout(cmdbuffer, image, format, VK_IMAGE_LAYOUT_UNDEFINED,
                   VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

  VkImageSubresourceRange subresourceRange = {0};
  subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = 1;
  subresourceRange.baseArrayLayer = 0;
  subresourceRange.layerCount = 1;

  VkImageViewCreateInfo color_attachment_view = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  color_attachment_view.format = format;
  color_attachment_view.components.r = VK_COMPONENT_SWIZZLE_R;
  color_attachment_view.components.g = VK_COMPONENT_SWIZZLE_G;
  color_attachment_view.components.b = VK_COMPONENT_SWIZZLE_B;
  color_attachment_view.components.a = VK_COMPONENT_SWIZZLE_A;
  color_attachment_view.subresourceRange = subresourceRange;
  color_attachment_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
  color_attachment_view.image = image;

  SwapchainBuffers buffer;
  buffer.image = image;

  VK_CHECK_RESULT(vkCreateImageView(device, &color_attachment_view, NULL, &buffer.view));
  return buffer;
}

VkRenderPass createRenderPass(VkDevice device, VkFormat color_format, VkFormat depth_format) {
  VkAttachmentDescription attachments[2] = {{0}};
  attachments[0].format = color_format;
  attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
  attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  attachments[1].format = depth_format;
  attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
  attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attachments[1].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_reference = {0};
  color_reference.attachment = 0;
  color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depth_reference = {0};
  depth_reference.attachment = 1;
  depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = {0};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.flags = 0;
  subpass.inputAttachmentCount = 0;
  subpass.pInputAttachments = NULL;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_reference;
  subpass.pResolveAttachments = NULL;
  subpass.pDepthStencilAttachment = &depth_reference;
  subpass.preserveAttachmentCount = 0;
  subpass.pPreserveAttachments = NULL;

  VkRenderPassCreateInfo rp_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
  rp_info.attachmentCount = 2;
  rp_info.pAttachments = attachments;
  rp_info.subpassCount = 1;
  rp_info.pSubpasses = &subpass;
  VkRenderPass render_pass;
  VK_CHECK_RESULT(vkCreateRenderPass(device, &rp_info, NULL, &render_pass));
  return render_pass;
}

FrameBuffers createFrameBuffers(const VulkanDevice* device, VkCommandPool commandPool, VkSurfaceKHR surface, VkQueue queue, 
                                VkExtent2D windowExtent, VkSwapchainKHR oldSwapchain) {
  VkBool32 supportsPresent;
  vkGetPhysicalDeviceSurfaceSupportKHR(device->gpu, device->graphicsQueueFamilyIndex, surface, &supportsPresent);
  if (!supportsPresent) {
    exit(-1); // does not supported.
  }

  VkCommandBuffer setup_cmd_buffer = createAndBeginLocalCommandBuffer(device->device, commandPool);

  VkFormat colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
  VkColorSpaceKHR colorSpace;
  {
    // Get the list of VkFormats that are supported:
    uint32_t formatCount;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device->gpu, surface, &formatCount, NULL));
    VkSurfaceFormatKHR* surfFormats = (VkSurfaceFormatKHR*)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device->gpu, surface, &formatCount, surfFormats));
    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
      colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
    } else {
      assert(formatCount >= 1);
      colorFormat = surfFormats[0].format;
    }
    colorSpace = surfFormats[0].colorSpace;
    free(surfFormats);
  }

  // Check the surface capabilities and formats
  VkSurfaceCapabilitiesKHR surfCapabilities;
  VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->gpu, surface, &surfCapabilities));

  VkExtent2D buffer_size;
  // width and height are either both -1, or both not -1.
  if (surfCapabilities.currentExtent.width == (uint32_t)-1) {
    buffer_size = windowExtent;
  } else {
    // If the surface size is defined, the swap chain size must match
    buffer_size = surfCapabilities.currentExtent;
  }

  DepthBuffer depth = createDepthBuffer(device, buffer_size.width, buffer_size.height, VK_FORMAT_D24_UNORM_S8_UINT);

  VkRenderPass render_pass = createRenderPass(device->device, colorFormat, depth.format);

  VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device->gpu, surface, &presentModeCount, NULL);
  assert(presentModeCount > 0);

  VkPresentModeKHR* presentModes = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) * presentModeCount);
  vkGetPhysicalDeviceSurfacePresentModesKHR(device->gpu, surface, &presentModeCount, presentModes);

  for (size_t i = 0; i < presentModeCount; i++) {
    if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
      swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
      break;
    }
    if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
      swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    }
  }
  free(presentModes);

  VkSurfaceTransformFlagBitsKHR preTransform;
  if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
    preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  } else {
    preTransform = surfCapabilities.currentTransform;
  }

  // Determine the number of VkImage's to use in the swap chain (we desire to
  // own only 1 image at a time, besides the images being displayed and
  // queued for display):
  uint32_t desiredNumberOfSwapchainImages = surfCapabilities.minImageCount + 1;
  if ((surfCapabilities.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfCapabilities.maxImageCount)) {
    // Application must settle for fewer images than desired:
    desiredNumberOfSwapchainImages = surfCapabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR swapchainInfo = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
  swapchainInfo.surface = surface;
  swapchainInfo.minImageCount = desiredNumberOfSwapchainImages;
  swapchainInfo.imageFormat = colorFormat;
  swapchainInfo.imageColorSpace = colorSpace;
  swapchainInfo.imageExtent = buffer_size;
  swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainInfo.preTransform = preTransform;
  swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainInfo.imageArrayLayers = 1;
  swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainInfo.presentMode = swapchainPresentMode;
  swapchainInfo.oldSwapchain = oldSwapchain;
  swapchainInfo.clipped = VK_TRUE;

  VkSwapchainKHR swap_chain;
  VK_CHECK_RESULT(vkCreateSwapchainKHR(device->device, &swapchainInfo, NULL, &swap_chain));

  if (oldSwapchain != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(device->device, oldSwapchain, NULL);
  }

  uint32_t swapchain_image_count;
  VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device->device, swap_chain, &swapchain_image_count, NULL));

  VkImage* swapchainImages = (VkImage*)malloc(swapchain_image_count * sizeof(VkImage));

  assert(swapchainImages);

  VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device->device, swap_chain, &swapchain_image_count, swapchainImages));

  SwapchainBuffers* swap_chain_buffers = (SwapchainBuffers*)malloc(swapchain_image_count * sizeof(SwapchainBuffers));
  for (uint32_t i = 0; i < swapchain_image_count; i++) {
    swap_chain_buffers[i] = createSwapchainBuffers(device->device, colorFormat, setup_cmd_buffer, swapchainImages[i]);
  }
  free(swapchainImages);

  VkImageView attachments[2];
  attachments[1] = depth.view;

  VkFramebufferCreateInfo fb_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
  fb_info.renderPass = render_pass;
  fb_info.attachmentCount = 2;
  fb_info.pAttachments = attachments;
  fb_info.width = buffer_size.width;
  fb_info.height = buffer_size.height;
  fb_info.layers = 1;
  uint32_t i;

  VkFramebuffer* framebuffers = (VkFramebuffer*)malloc(swapchain_image_count * sizeof(VkFramebuffer));
  assert(framebuffers);

  for (i = 0; i < swapchain_image_count; i++) {
    attachments[0] = swap_chain_buffers[i].view;
    VK_CHECK_RESULT(vkCreateFramebuffer(device->device, &fb_info, NULL, &framebuffers[i]));
  }

  endCommandAndSubmitToQueue(setup_cmd_buffer, queue);
  vkQueueWaitIdle(queue);

  vkFreeCommandBuffers(device->device, commandPool, 1, &setup_cmd_buffer);

  FrameBuffers buffer = {0};
  buffer.swap_chain = swap_chain;
  buffer.swap_chain_buffers = swap_chain_buffers;
  buffer.swapchain_image_count = swapchain_image_count;
  buffer.framebuffers = framebuffers;
  buffer.current_buffer = 0;
  buffer.format = colorFormat;
  buffer.buffer_size = buffer_size;
  buffer.render_pass = render_pass;
  buffer.depth = depth;

  return buffer;
}
void destroyFrameBuffers(const VulkanDevice* device, FrameBuffers* buffer) {
  for (int i = 0; i < buffer->swapchain_image_count; ++i) {
    vkDestroyImageView(device->device, buffer->swap_chain_buffers[i].view, 0);
    vkDestroyFramebuffer(device->device, buffer->framebuffers[i], 0);
  }

  vkDestroyImageView(device->device, buffer->depth.view, 0);
  vkDestroyImage(device->device, buffer->depth.image, 0);
  vkFreeMemory(device->device, buffer->depth.mem, 0);

  vkDestroyRenderPass(device->device, buffer->render_pass, 0);
  vkDestroySwapchainKHR(device->device, buffer->swap_chain, 0);

  free(buffer->framebuffers);
  free(buffer->swap_chain_buffers);
}
