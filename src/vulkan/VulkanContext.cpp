#include "VulkanContext.hpp"
#include "vku.hpp"
#define NANOVG_VULKAN_IMPLEMENTATION
#include "nanovg_vk.h"
#include "core/Log.hpp"
#include "core/MathDef.hpp"
#include "core/Status.hpp"

namespace fui {

struct VulkanContext::Private {
  std::shared_ptr<vk::UniqueInstance> instance;
  vk::SurfaceKHR surface;
  std::shared_ptr<vk::su::SwapChainData> swapchain;
  vk::PhysicalDevice gpu;
  vk::UniqueDevice device;
  vk::UniqueCommandPool commandPool;
  vk::Queue graphicsQueue;
  vk::UniqueCommandBuffer commandBuffer;
  vk::UniqueSemaphore presentComplete;
  vk::UniqueSemaphore renderComplete;
  vk::UniqueRenderPass renderPass;
  std::vector<vk::UniqueFramebuffer> framebuffers;
  uint32_t currentBuffer;
  vk::Extent2D frameExtent;
  std::shared_ptr<vk::su::DepthBufferData> depthBuffer;
  std::shared_ptr<vk::UniqueDebugReportCallbackEXT> debugCallback;
};

VulkanContext::VulkanContext(const std::shared_ptr<vk::UniqueInstance>& instance,
                             const std::shared_ptr<vk::UniqueDebugReportCallbackEXT>& debugReportCallback) {
  _p.reset(new Private);
  _p->instance = instance;
  _p->debugCallback = debugReportCallback;
  auto gpuResult = instance->get().enumeratePhysicalDevices();
  if (!gpuResult.empty()) {
    _p->gpu = gpuResult[0];
  } else {
    LOGE << "vkEnumeratePhysicalDevices failed ";
  }
  auto queueFamilyProperties = _p->gpu.getQueueFamilyProperties();
  auto graphicsQueueFamilyIndex = vk::su::findGraphicsQueueFamilyIndex(queueFamilyProperties);

  constexpr float queuePriorities[] = {0.f};
  vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), graphicsQueueFamilyIndex, 1,
                                                  queuePriorities);
  const char* deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  constexpr auto deviceExtensionsCount = sizeof(deviceExtensions) / sizeof(deviceExtensions[0]);
  vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(), 1, &deviceQueueCreateInfo, 0, nullptr,
                                        deviceExtensionsCount, deviceExtensions);
  _p->device = _p->gpu.createDeviceUnique(deviceCreateInfo);

  auto d = _p->device.get();

  /* Create a command pool to allocate our command buffer from */
  _p->commandPool =
      d.createCommandPoolUnique({vk::CommandPoolCreateFlagBits::eResetCommandBuffer, graphicsQueueFamilyIndex});
  _p->graphicsQueue = d.getQueue(graphicsQueueFamilyIndex, 0);

  auto commandBuffers = d.allocateCommandBuffersUnique({_p->commandPool.get(), vk::CommandBufferLevel::ePrimary, 1});
  _p->commandBuffer = std::move(commandBuffers[0]);
}

VulkanContext::~VulkanContext() {
  nvgDeleteVk(vg());

  _p->renderComplete.reset();
  _p->presentComplete.reset();

  _p->framebuffers.clear();
  _p->renderPass.reset();
  _p->depthBuffer.reset();
  _p->swapchain.reset();

  // destroyFrameBuffers(_p->device.get(), &_p->frameBuffer);

  _p->commandBuffer.reset();
  _p->commandPool.reset();

  _p->device.reset();

  _p->instance->get().destroySurfaceKHR(_p->surface, NULL);
}

Status VulkanContext::initSwapchain(const vk::SurfaceKHR& surface, const vk::Extent2D& extent) {
  auto queueIndices = vk::su::findGraphicsAndPresentQueueFamilyIndex(_p->gpu, surface);
  auto graphicsQueueFamilyIndex = queueIndices.first;
  auto presentQueueFamilyIndex = queueIndices.second;
  auto oldSwapChainData = _p->swapchain ? std::move(_p->swapchain->swapChain) : vk::UniqueSwapchainKHR();
  _p->surface = surface;
  _p->frameExtent = extent;
  _p->swapchain = std::make_shared<vk::su::SwapChainData>(
      _p->gpu, _p->device, surface, _p->frameExtent,
      vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc, oldSwapChainData,
      graphicsQueueFamilyIndex, presentQueueFamilyIndex);
  return Status::OK;
}

Status VulkanContext::initFramebuffer(const vk::Extent2D& extent) {
  vk::Format depthFormat = vk::su::pickDepthFormat(_p->gpu);

  auto colorFormat = _p->swapchain->colorFormat;

  _p->renderPass = vk::su::createRenderPass(_p->device, colorFormat, depthFormat);
  _p->presentComplete = _p->device->createSemaphoreUnique(vk::SemaphoreCreateInfo());
  _p->renderComplete = _p->device->createSemaphoreUnique(vk::SemaphoreCreateInfo());

  _p->depthBuffer = std::make_shared<vk::su::DepthBufferData>(_p->gpu, _p->device, depthFormat, extent);

  vk::su::oneTimeSubmit(_p->commandBuffer, _p->graphicsQueue, [&](vk::UniqueCommandBuffer const& commandBuffer) {
    vk::su::setImageLayout(commandBuffer, *_p->depthBuffer->image, depthFormat, vk::ImageLayout::eUndefined,
                           vk::ImageLayout::eDepthStencilAttachmentOptimal);
  });

  _p->framebuffers = vk::su::createFramebuffers(_p->device, _p->renderPass, _p->swapchain->imageViews,
                                                _p->depthBuffer->imageView, extent);

  return Status::OK;
}

Status VulkanContext::initVG() {
  if (_vg != nullptr) { nvgDeleteVk(_vg); }
  VKNVGCreateInfo createInfo = {0};
  createInfo.gpu = _p->gpu;
  createInfo.device = _p->device.get();
  createInfo.commandPool = _p->commandPool.get();
  createInfo.queue = _p->graphicsQueue;
  createInfo.renderpass = _p->renderPass.get();
  createInfo.cmdBuffer = _p->commandBuffer.get();
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

Status VulkanContext::rebuildSwapchain() {
  auto extent = _p->frameExtent;

  _p->device->waitIdle();
  initSwapchain(_p->surface, extent);

  vk::Format depthFormat = _p->depthBuffer->format;

  *(_p->depthBuffer) = vk::su::DepthBufferData(_p->gpu, _p->device, depthFormat, extent);

  vk::su::oneTimeSubmit(_p->commandBuffer, _p->graphicsQueue, [&](vk::UniqueCommandBuffer const& commandBuffer) {
    vk::su::setImageLayout(commandBuffer, *_p->depthBuffer->image, depthFormat, vk::ImageLayout::eUndefined,
                           vk::ImageLayout::eDepthStencilAttachmentOptimal);
  });

  _p->framebuffers = vk::su::createFramebuffers(_p->device, _p->renderPass, _p->swapchain->imageViews,
                                                _p->depthBuffer->imageView, extent);
  return Status::OK;
}

auto VulkanContext::setViewport(int x, int y, int width, int height) -> decltype(this) {
  vk::Viewport viewport = {(float)x, (float)y, (float)width, (float)height, 0.f, 1.f};
  vk::Rect2D scissor = {{x, y}, {(uint32_t)width, (uint32_t)height}};
  _p->commandBuffer->setViewport(0, 1, &viewport);
  _p->commandBuffer->setScissor(0, 1, &scissor);
  return this;
}

auto VulkanContext::preDraw(const Recti& renderArea, const Color* clearColor, const float* clearDepth,
                            const int* clearStencil) -> decltype(this) {
  if (renderArea.w != _p->frameExtent.width || renderArea.h != _p->frameExtent.height) {
    _p->frameExtent = vk::Extent2D(renderArea.w, renderArea.h);
    rebuildSwapchain();
  }
  auto&& cb = _p->currentBuffer;

  // Get the index of the next available swapchain image:
  _p->device->acquireNextImageKHR(_p->swapchain->swapChain.get(), vk::su::FenceTimeout, _p->presentComplete.get(),
                                  nullptr, &cb);

  if (clearColor != nullptr) {}
  if (clearDepth != nullptr) {}
  if (clearStencil != nullptr) {}
  vk::ClearValue clearValues[2];
  clearValues[0].color.float32[0] = 0.3f;
  clearValues[0].color.float32[1] = 0.3f;
  clearValues[0].color.float32[2] = 0.32f;
  clearValues[0].color.float32[3] = 1.0f;
  clearValues[1].depthStencil.depth = 1.0f;
  clearValues[1].depthStencil.stencil = 0;

  vk::RenderPassBeginInfo renderPassBeginInfo;
  renderPassBeginInfo.pNext = NULL;
  renderPassBeginInfo.renderPass = _p->renderPass.get();
  renderPassBeginInfo.framebuffer = _p->framebuffers[cb].get();
  renderPassBeginInfo.renderArea.offset.x = renderArea.x;
  renderPassBeginInfo.renderArea.offset.y = renderArea.y;
  renderPassBeginInfo.renderArea.extent.width = renderArea.w;
  renderPassBeginInfo.renderArea.extent.height = renderArea.h;
  renderPassBeginInfo.clearValueCount = 2;
  renderPassBeginInfo.pClearValues = clearValues;

  _p->commandBuffer->begin(vk::CommandBufferBeginInfo());
  _p->commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

  return setViewport(renderArea.x, renderArea.y, renderArea.w, renderArea.h);
}

auto VulkanContext::postDraw() -> decltype(this) {
  auto&& cb = _p->currentBuffer;

  _p->commandBuffer->endRenderPass();
  _p->commandBuffer->end();

  vk::PipelineStageFlags pipelineStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;

  vk::SubmitInfo submitInfo(1, &_p->presentComplete.get(), &pipelineStageFlags, 1, &_p->commandBuffer.get(), 1,
                            &_p->renderComplete.get());

  /* Queue the command buffer for execution */
  _p->graphicsQueue.submit(submitInfo, nullptr);

  /* Now present the image in the window */
  vk::PresentInfoKHR presentInfo(1, &_p->renderComplete.get(), 1, &_p->swapchain->swapChain.get(), &cb);
  try {
    _p->graphicsQueue.presentKHR(presentInfo);
    _p->graphicsQueue.waitIdle();
  } catch (const vk::OutOfDateKHRError&) {}

  return this;
}

std::string VulkanContext::versionLine() const { return "#version 400"; }

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
