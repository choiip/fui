#include <iostream>
#include <chrono>

#include <core/ApplicationContext.hpp>
#include <core/Status.hpp>
#include <GL/GlfwRenderWindow.hpp>
#include <GL/GL3Context.hpp>
#include <GL/glad.h>
using namespace fui;

#include "demo.h"
#include "perf.h"

class ExampleApp : public ApplicationContext {
private:
    typedef std::chrono::steady_clock clock;
    typedef clock::time_point time_point;
public:
    ExampleApp(NVGcontext* vg)
    : vg(vg)
    , cpuTime(0)
    {
    }

protected:
    virtual Status onEnter() override {
        if (loadDemoData(vg, &data) == -1) {
            std::cerr << "loadDemoData error\n";
            return Status::UNKNOWN_ERROR;
        }
        // initGPUTimer(&gpuTimer);
        prevt = clock::now();
        
        return Status::OK;
    }

    virtual void onDraw() override {
        int mx, my;
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

		// startGPUTimer(&gpuTimer);

		window->getCursorPos(mx, my);
		window->getWindowSize(winWidth, winHeight);
		window->getDrawableSize(fbWidth, fbHeight);
		// Calculate pixel ration for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;

		// Update and render
		glViewport(0, 0, fbWidth, fbHeight);
		// if (premult)
		// 	glClearColor(0,0,0,0);
		// else
			glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

        auto blowup = 0;
		renderDemo(vg, mx,my, winWidth, winHeight, t.time_since_epoch().count(), blowup, &data);

		renderGraph(vg, 5,5, &fps);
		renderGraph(vg, 5+200+5,5, &cpuGraph);
		// if (gpuTimer.supported)
		// 	renderGraph(vg, 5+200+5+200+5,5, &gpuGraph);

		nvgEndFrame(vg);

		// Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
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

    virtual void onExit() override {
        freeDemoData(vg, &data);
    }
private:
	DemoData data;
    NVGcontext* vg;
    PerfGraph fps, cpuGraph, gpuGraph;
    time_point prevt;
    double cpuTime;
};

int main() {
    GlfwRenderWindow window;
    GL3Context glContext;

	auto st = window.create(1000, 600);
    if (st != Status::OK) return -1;
    glContext.init(&window);

	window.setSwapInterval(0);

    ExampleApp app(glContext.vg());
    app.run(window);

    glContext.deinit();
    window.destroy();
    return 0;
}
