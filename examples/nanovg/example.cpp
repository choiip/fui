#include <chrono>
#include <memory>
#include <fui.hpp>
#include <GL/GL3Context.hpp>
#ifdef FUI_ENABLE_VULKAN
#include <vulkan/VulkanContext.hpp>
#endif
using namespace fui;
#include <GL/header.h>

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
  }
  virtual ~ExampleApp() {}

protected:
  virtual Status onEnter() override {
    // LOGD << glGetString(GL_VERSION);
    if (loadDemoData(vg, &data) == -1) {
      LOGE << "loadDemoData error";
      return Status::UNKNOWN_ERROR;
    }

    initGraph(&fps, GRAPH_RENDER_FPS, "FPS");
    initGraph(&cpuGraph, GRAPH_RENDER_MS, "CPU");

    // window->setSwapInterval(0);
    // initGPUTimer(&gpuTimer);
    prevt = clock::now();
    window->show();

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
    int winWidth = window->size().x;
    int winHeight = window->size().y;
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

  virtual void onExit() override { /*freeDemoData(vg, &data);*/
  }

  RenderWindow* window;
  NVGcontext* vg;
  DemoData data;
  PerfoGraph fps, cpuGraph, gpuGraph;
  time_point prevt;
  double cpuTime;
  int mx, my;
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

    winWidth = window->size().x;
    winHeight = window->size().y;
    window->getDrawableSize(fbWidth, fbHeight);
    // Calculate pixel ration for hi-dpi devices.
    pxRatio = (float)fbWidth / (float)winWidth;

    _vulkanContext->preDraw({0, 0, fbWidth, fbHeight});

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

    _vulkanContext->postDraw();
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
  if (!graphicsProfile) return -1;

  auto window = windowManager.createWindow(1000, 600, *graphicsProfile);
  delete graphicsProfile;

  if (!window) return -1;

  std::unique_ptr<ApplicationContext> app(new ExampleApp(window));
  app->run(windowManager);

  return 0;
}
