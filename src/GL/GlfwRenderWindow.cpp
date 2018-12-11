#include "GL/GlfwRenderWindow.hpp"
#include <GLFW/glfw3.h>

namespace fui {

GlfwRenderWindow::GlfwRenderWindow(GLFWwindow* window)
: _window(window)
{}

GlfwRenderWindow::~GlfwRenderWindow() {
    if (_window) {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
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
