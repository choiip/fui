#include "GlfwVulkanProfile.hpp"
#include <iostream>
#include <memory>
#if defined(__APPLE__)
	#include <vulkan/vulkan.h>
	#define GLFW_INCLUDE_NONE
#else
	#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#include "vku.h"

#define NANOVG_VULKAN_IMPLEMENTATION
#include "nanovg_vk.h"

#include "VulkanContext.hpp"

namespace fui {

GlfwVulkanProfile::~GlfwVulkanProfile() {

}

void GlfwVulkanProfile::prepare() const {
    if (!glfwVulkanSupported()) {
        std::cerr << ("GLFW failed to find the Vulkan loader.\n");
    }
	
    //// https://www.glfw.org/docs/latest/window_guide.html#window_hints
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

RenderContext* GlfwVulkanProfile::createContext(void* nativeWindow) const {
	GLFWwindow* window = (GLFWwindow*)nativeWindow;

	VulkanContext::Resource resource;

#ifdef NDEBUG
	resource.instance = createVkInstance(false);
#else
	resource.instance = createVkInstance(true);
	resource.debugCallback = CreateDebugReport(resource.instance);
#endif

	VkResult res;
	res = glfwCreateWindowSurface(resource.instance, window, 0, &resource.surface);
	if (VK_SUCCESS != res) {
		std::cerr << ("glfwCreateWindowSurface failed\n");
		return nullptr;
	}

	uint32_t gpu_count = 1;
	res = vkEnumeratePhysicalDevices(resource.instance, &gpu_count, &resource.gpu);
	if (res != VK_SUCCESS && res != VK_INCOMPLETE) {
		std::cerr << "vkEnumeratePhysicalDevices failed " << res << " \n";
		return nullptr;
	}
	auto device = resource.device = createVulkanDevice(resource.gpu);

	int winWidth, winHeight;
	glfwGetWindowSize(window, &winWidth, &winHeight);

	vkGetDeviceQueue(device->device, device->graphicsQueueFamilyIndex, 0, &resource.queue);
	resource.frameBuffer = createFrameBuffers(device, resource.surface, resource.queue, winWidth, winHeight, 0);
	resource.cmdBuffer = createCmdBuffer(device->device, device->commandPool);

	VKNVGCreateInfo createInfo = {0};
	createInfo.device = device->device;
	createInfo.gpu = device->gpu;
	createInfo.renderpass = resource.frameBuffer.render_pass;
	createInfo.cmdBuffer = resource.cmdBuffer;

    auto vg = nvgCreateVk(createInfo, NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (vg == NULL) {
		std::cerr << ("Could not init nanovg.\n");
		return nullptr;
	}

	std::unique_ptr<VulkanContext> context(new VulkanContext(vg, resource));
	if (!context) {
		nvgDeleteVk(vg);
		std::cerr << ("Could not create render context.\n");
		return nullptr;
	}

	return context.release();
}

}
