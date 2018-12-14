#include "core/GlfwRenderWindow.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "core/RenderContext.hpp"

namespace fui {

GlfwRenderWindow::GlfwRenderWindow(GLFWwindow* window, RenderContext* renderContext)
: _window(window)
, _renderContext(renderContext)
{}

GlfwRenderWindow::~GlfwRenderWindow() {
    if (_window) {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
	delete _renderContext;
}

void* GlfwRenderWindow::nativeWindow() const {
	return _window;
}

RenderContext* GlfwRenderWindow::renderContext() const {
	return _renderContext;
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

void GlfwRenderWindow::hide() {
	glfwHideWindow(_window);
}

void GlfwRenderWindow::show() {
	glfwShowWindow(_window);
}

void GlfwRenderWindow::setSwapInterval(int interval) {
    glfwSwapInterval(interval);
}

void GlfwRenderWindow::swapBuffer() {
    glfwSwapBuffers(_window);
}

}
