#include "core/GlfwWindowManager.hpp"
#include <iostream>
#include <GL/glad.h>
#include <GLFW/glfw3.h>
#include "GL/GlfwRenderWindow.hpp"

namespace fui {

template<> GlfwWindowManager* Singleton<GlfwWindowManager>::_singleton = 0; 

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const auto& windows = GlfwWindowManager::instance().getWindows();
    auto targetWindow = windows.find(window);
    if (targetWindow != windows.end()) {

    }
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    const auto& windows = GlfwWindowManager::instance().getWindows();
    auto targetWindow = windows.find(window);
    if (targetWindow != windows.end()) {

    }
}

GlfwWindowManager::GlfwWindowManager() {
    if (!glfwInit()) {
		std::cerr << ("Failed to init GLFW.\n");
	}
}

GlfwWindowManager::~GlfwWindowManager() {
    glfwTerminate();
}

RenderWindow* GlfwWindowManager::createWindow(int width, int height) {
#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	auto glfwWindow = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (!glfwWindow) {
        std::cerr << ("Could not create GLFW window!\n");
        return nullptr;
    } 

	glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << ("Could not initialize GLAD!\n");
        return nullptr;
    }
    glGetError();   // pull and ignore unhandled errors like GL_INVALID_ENUM

    // set callbacks
    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);

    return new GlfwRenderWindow(glfwWindow);
}

const std::unordered_map<GLFWwindow*, GlfwRenderWindow*>& GlfwWindowManager::getWindows() const {
    return _windows;
}

}
