#include "VulkanContext.hpp"
#include <vulkan/vulkan.h>
#include "vku.h"
#define NANOVG_VULKAN_IMPLEMENTATION
#include "nanovg_vk.h"

namespace fui {

VulkanContext::VulkanContext(NVGcontext* vg, Resource& resource)
: RenderContext(vg)
, _resource(resource)
{}

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

}
