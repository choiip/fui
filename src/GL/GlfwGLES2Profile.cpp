#include "GlfwGLES2Profile.hpp"
#include <iostream>
#include <memory>
#include <GL/glad.h>
#include <GLFW/glfw3.h>

#include "nanovg/nanovg.h"
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"

#include "GL/GLES2Context.hpp"

namespace fui {

void GlfwGLES2Profile::prepare() const {
    //// https://www.glfw.org/docs/latest/window_guide.html#window_hints
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
}

RenderContext* GlfwGLES2Profile::createContext() const {
	if (!gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << ("Could not initialize GLAD!\n");
        return nullptr;
    }
    glGetError();   // pull and ignore unhandled errors like GL_INVALID_ENUM

    auto vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (vg == NULL) {
		std::cerr << ("Could not init nanovg.\n");
		return nullptr;
	}

	std::unique_ptr<GLES2Context> context(new GLES2Context(vg));
	if (!context) {
		nvgDeleteGLES2(vg);
		std::cerr << ("Could not create render context.\n");
		return nullptr;
	}

	return context.release();
}

}
