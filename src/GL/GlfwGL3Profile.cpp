#include "GlfwGL3Profile.hpp"
#include <iostream>
#include <memory>
#include <GL/glad.h>
#include <GLFW/glfw3.h>

#include "nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include "GL/GL3Context.hpp"

namespace fui {

void GlfwGL3Profile::prepare() const {
    //// https://www.glfw.org/docs/latest/window_guide.html#window_hints
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
}

RenderContext* GlfwGL3Profile::createContext() const {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << ("Could not initialize GLAD!\n");
        return nullptr;
    }
    glGetError();   // pull and ignore unhandled errors like GL_INVALID_ENUM

    auto vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (vg == NULL) {
		std::cerr << ("Could not init nanovg.\n");
		return nullptr;
	}

	std::unique_ptr<GL3Context> context(new GL3Context(vg));
	if (!context) {
		nvgDeleteGL3(vg);
		std::cerr << ("Could not create render context.\n");
		return nullptr;
	}

	return context.release();
}

}
