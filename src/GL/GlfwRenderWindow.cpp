#include "GL/GlfwRenderWindow.hpp"
#include <iostream>
#include "GL/glad.h"
#include <GLFW/glfw3.h>

#include "core/Status.hpp"

namespace fui {

GlfwRenderWindow::~GlfwRenderWindow() {

}

Status GlfwRenderWindow::create(int width, int height) {
    if (!glfwInit()) {
		std::cerr << ("Failed to init GLFW.\n");
		return Status::UNKNOWN_ERROR;
	}

#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	_window = glfwCreateWindow(width, height, "Title", NULL, NULL);

	glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << ("Could not initialize GLAD!\n");
        return Status::UNKNOWN_ERROR;
    }
    glGetError();   // pull and ignore unhandled errors like GL_INVALID_ENUM

    return Status::OK;
}

void GlfwRenderWindow::destroy() {
    if (_window) {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
    glfwTerminate();
}

void* GlfwRenderWindow::nativeWindow() {
	return _window;
}

void GlfwRenderWindow::getCursorPos(int& x, int& y) {
	double dx, dy;
	glfwGetCursorPos(_window, &dx, &dy);
	x = (int)dx;
	y = (int)dy;
}

void GlfwRenderWindow::getWindowSize(int& width, int& height) {
	glfwGetWindowSize(_window, &width, &height);
}

void GlfwRenderWindow::getDrawableSize(int& width, int& height) {
	glfwGetFramebufferSize(_window, &width, &height);
}

void GlfwRenderWindow::setSwapInterval(int interval) {
    glfwSwapInterval(interval);
}

void GlfwRenderWindow::swapBuffer() {
    glfwSwapBuffers(_window);
}

}
