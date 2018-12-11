#include "GlfwGL3Profile.hpp"

#include <GLFW/glfw3.h>

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

}
