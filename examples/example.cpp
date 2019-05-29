#include <chrono>
#include <memory>
#include <fui.hpp>
#include <GL/GL3Context.hpp>
#ifdef FUI_ENABLE_VULKAN
#include <vulkan/VulkanContext.hpp>
#endif
using namespace fui;
#include <GL/glad.h>

#include "demo.h"
#include "perf.h"

class ExampleApp : public ApplicationContext {
protected:
  typedef std::chrono::steady_clock clock;
  typedef clock::time_point time_point;

public:
  ExampleApp(RenderWindow* renderWindow)
  : window(renderWindow)
  , vg(renderWindow->renderContext()->vg())
  , cpuTime(0) {
    renderWindow->onKey([renderWindow](Key key, ButtonAction action, Modifier mods) {
      if (key == Key::KEY_ESCAPE) {
        LOGD << "Window closing...";
        renderWindow->close();
      }
    });
    renderWindow->onMouseMove([this](int xpos, int ypos) {
      this->mx = xpos;
      this->my = ypos;
    });
    renderWindow->onResize([this](int width, int height) {
      winWidth = width;
      winHeight = height;
    });
  }
  virtual ~ExampleApp() {}

protected:
  virtual Status onEnter() override {
    if (loadDemoData(vg, &data) == -1) {
      LOGE << "loadDemoData error";
      return Status::UNKNOWN_ERROR;
    }

    // window->setSwapInterval(0);
    // initGPUTimer(&gpuTimer);
    prevt = clock::now();
    window->show();
    window->getWindowSize(winWidth, winHeight);
    return Status::OK;
  }

  virtual void onDraw() override {
    time_point t;
    double dt;

    int fbWidth, fbHeight;
    float pxRatio;
    float gpuTimes[3];
    int i, n;

    t = clock::now();
    dt = (t - prevt).count() / (double)(1000 * 1000 * 1000);
    prevt = t;

    // startGPUTimer(&gpuTimer);

    window->getDrawableSize(fbWidth, fbHeight);
    // Calculate pixel ration for hi-dpi devices.
    pxRatio = (float)fbWidth / (float)winWidth;

    // Update and render
    glViewport(0, 0, fbWidth, fbHeight);
    glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

    auto blowup = 0;
    renderDemo(vg, mx, my, winWidth, winHeight, t.time_since_epoch().count(), blowup, &data);

    renderGraph(vg, 5, 5, &fps);
    renderGraph(vg, 5 + 200 + 5, 5, &cpuGraph);
    // if (gpuTimer.supported)
    // 	renderGraph(vg, 5+200+5+200+5,5, &gpuGraph);

    nvgEndFrame(vg);

    // Measure the CPU time taken excluding swap buffers (as the swap may wait
    // for GPU)
    cpuTime = (clock::now() - t).count() / (double)(1000 * 1000 * 1000);

    updateGraph(&fps, dt);
    updateGraph(&cpuGraph, cpuTime);

    // We may get multiple results.
    // n = stopGPUTimer(&gpuTimer, gpuTimes, 3);
    // for (i = 0; i < n; i++)
    // 	updateGraph(&gpuGraph, gpuTimes[i]);

    // if (screenshot) {
    // 	screenshot = 0;
    // 	saveScreenShot(fbWidth, fbHeight, premult, "dump.png");
    // }

    window->swapBuffer();
  }

  virtual void onExit() override { freeDemoData(vg, &data); }

  RenderWindow* window;
  NVGcontext* vg;
  DemoData data;
  PerfGraph fps, cpuGraph, gpuGraph;
  time_point prevt;
  double cpuTime;
  int mx, my, winWidth = 0, winHeight = 0;
};
#ifdef FUI_ENABLE_VULKAN
class VulkanExampleApp : public ExampleApp {
public:
  VulkanExampleApp(RenderWindow* renderWindow)
  : ExampleApp(renderWindow) {
    _vulkanContext = static_cast<VulkanContext*>(renderWindow->renderContext());
  }
  virtual ~VulkanExampleApp() {}

protected:
  virtual void onDraw() override {
    time_point t;
    double dt;
    int winWidth, winHeight;
    int fbWidth, fbHeight;
    float pxRatio;
    float gpuTimes[3];
    int i, n;

    t = clock::now();
    dt = (t - prevt).count() / (double)(1000 * 1000 * 1000);
    prevt = t;

    window->getWindowSize(winWidth, winHeight);
    window->getDrawableSize(fbWidth, fbHeight);
    // Calculate pixel ration for hi-dpi devices.
    pxRatio = (float)fbWidth / (float)winWidth;

    auto device = _vulkanContext->device();
    auto queue = _vulkanContext->queue();
    auto cmdBuffer = _vulkanContext->cmdBuffer();
    auto frameBuffer = _vulkanContext->frameBuffer();

    prepareFrame(device->device, cmdBuffer, frameBuffer);

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

    auto blowup = 0;
    renderDemo(vg, mx, my, winWidth, winHeight, t.time_since_epoch().count(), blowup, &data);

    renderGraph(vg, 5, 5, &fps);
    renderGraph(vg, 5 + 200 + 5, 5, &cpuGraph);
    // if (gpuTimer.supported)
    // 	renderGraph(vg, 5+200+5+200+5,5, &gpuGraph);

    nvgEndFrame(vg);

    // Measure the CPU time taken excluding swap buffers (as the swap may wait
    // for GPU)
    cpuTime = (clock::now() - t).count() / (double)(1000 * 1000 * 1000);

    updateGraph(&fps, dt);
    updateGraph(&cpuGraph, cpuTime);

    submitFrame(device->device, queue, cmdBuffer, frameBuffer);
  }

private:
  void prepareFrame(VkDevice device, VkCommandBuffer cmd_buffer, FrameBuffers* fb) {
    VkResult res;

    // Get the index of the next available swapchain image:
    res = vkAcquireNextImageKHR(device, fb->swap_chain, UINT64_MAX, fb->present_complete_semaphore, 0,
                                &fb->current_buffer);
    assert(res == VK_SUCCESS);

    const VkCommandBufferBeginInfo cmd_buf_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    res = vkBeginCommandBuffer(cmd_buffer, &cmd_buf_info);
    assert(res == VK_SUCCESS);

    VkClearValue clear_values[2];
    clear_values[0].color.float32[0] = 0.3f;
    clear_values[0].color.float32[1] = 0.3f;
    clear_values[0].color.float32[2] = 0.32f;
    clear_values[0].color.float32[3] = 1.0f;
    clear_values[1].depthStencil.depth = 1.0f;
    clear_values[1].depthStencil.stencil = 0;

    VkRenderPassBeginInfo rp_begin;
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = fb->render_pass;
    rp_begin.framebuffer = fb->framebuffers[fb->current_buffer];
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = fb->buffer_size.width;
    rp_begin.renderArea.extent.height = fb->buffer_size.height;
    rp_begin.clearValueCount = 2;
    rp_begin.pClearValues = clear_values;

    vkCmdBeginRenderPass(cmd_buffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport;
    viewport.width = fb->buffer_size.width;
    viewport.height = fb->buffer_size.height;
    viewport.minDepth = (float)0.0f;
    viewport.maxDepth = (float)1.0f;
    viewport.x = rp_begin.renderArea.offset.x;
    viewport.y = rp_begin.renderArea.offset.y;
    vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);

    VkRect2D scissor = rp_begin.renderArea;
    vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
  }

  void submitFrame(VkDevice device, VkQueue queue, VkCommandBuffer cmd_buffer, FrameBuffers* fb) {
    VkResult res;

    vkCmdEndRenderPass(cmd_buffer);

    vkEndCommandBuffer(cmd_buffer);

    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.pNext = NULL;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &fb->present_complete_semaphore;
    submit_info.pWaitDstStageMask = &pipe_stage_flags;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &cmd_buffer;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &fb->render_complete_semaphore;

    /* Queue the command buffer for execution */
    res = vkQueueSubmit(queue, 1, &submit_info, 0);
    assert(res == VK_SUCCESS);

    /* Now present the image in the window */

    VkPresentInfoKHR present = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    present.pNext = NULL;
    present.swapchainCount = 1;
    present.pSwapchains = &fb->swap_chain;
    present.pImageIndices = &fb->current_buffer;
    present.waitSemaphoreCount = 1;
    present.pWaitSemaphores = &fb->render_complete_semaphore;

    res = vkQueuePresentKHR(queue, &present);
    assert(res == VK_SUCCESS);

    res = vkQueueWaitIdle(queue);
  }

private:
  VulkanContext* _vulkanContext;
};
#endif

int main() {
  Logger logger;
  GlfwWindowManager windowManager;

  auto graphicAPI = GraphicsAPI::OPENGL;
#ifdef __EMSCRIPTEN__
  graphicAPI = GraphicsAPI::OPENGL_ES;
#endif
  auto graphicsProfile = windowManager.createGraphicsProfile(graphicAPI, 3, 2);
  if (!graphicsProfile)
    return -1;

  auto window = windowManager.createWindow(1000, 600, *graphicsProfile);
  delete graphicsProfile;

  if (!window)
    return -1;

  std::unique_ptr<ApplicationContext> app(new ExampleApp(window));
  app->run(windowManager);

  return 0;
}
