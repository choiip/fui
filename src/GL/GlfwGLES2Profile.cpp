#include "GlfwGLES2Profile.hpp"

#include <GLFW/glfw3.h>

namespace fui {

void GlfwGLES2Profile::prepare() const {
    //// https://www.glfw.org/docs/latest/window_guide.html#window_hints
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
}

}
