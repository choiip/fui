#pragma once

#include <vulkan/vulkan.h>
#include "core/Log.hpp"

namespace fui {

VkBool32 vulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*objectType*/,
                                   uint64_t /*object*/, size_t /*location*/, int32_t /*messageCode*/,
                                   const char* /*pLayerPrefix*/, const char* pMessage, void* /*pUserData*/) {
  switch (flags) {
  case VK_DEBUG_REPORT_INFORMATION_BIT_EXT: LOGD << "INFORMATION: " << pMessage; break;
  case VK_DEBUG_REPORT_WARNING_BIT_EXT: LOGW << "WARNING: " << pMessage; break;
  case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT: LOGW << "PERFORMANCE WARNING: " << pMessage; break;
  case VK_DEBUG_REPORT_ERROR_BIT_EXT: LOGE << "ERROR: " << pMessage; break;
  case VK_DEBUG_REPORT_DEBUG_BIT_EXT: LOGD << "DEBUG: " << pMessage; break;
  default: LOGE << "unknown flag (" << flags << "): " << pMessage; break;
  }
  return VK_TRUE;
}

} // namespace fui
