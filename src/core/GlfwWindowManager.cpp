#include "core/GlfwWindowManager.hpp"
#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>
#include "core/RenderContext.hpp"
#include "GL/GlfwGL3Profile.hpp"
#include "GL/GlfwGLES2Profile.hpp"
#include "GL/GlfwRenderWindow.hpp"

namespace fui {

template<> GlfwWindowManager* Singleton<GlfwWindowManager>::_singleton = 0; 

static void errorCallback(int error, const char* description) {
    std::cerr << ("GlfwWindowManager error: %s\n", description);
}

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
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
		std::cerr << ("Failed to init GLFW.\n");
	}
}

GlfwWindowManager::~GlfwWindowManager() {
    glfwTerminate();
}

RenderWindow* GlfwWindowManager::createWindow(int width, int height, const GraphicsProfile& graphicsProfile) {
    graphicsProfile.prepare();

	auto glfwWindow = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (!glfwWindow) {
        std::cerr << ("Could not create GLFW window!\n");
        return nullptr;
    } 

	glfwMakeContextCurrent(glfwWindow);

    auto renderContext = graphicsProfile.createContext(glfwWindow);
    if (!renderContext) {
        glfwDestroyWindow(glfwWindow);
        std::cerr << ("Could not create render context!\n");
        return nullptr;       
    }

    std::unique_ptr<GlfwRenderWindow> renderWindow(new GlfwRenderWindow(glfwWindow, renderContext));
    if (!renderWindow) {
        delete renderContext;
        glfwDestroyWindow(glfwWindow);
        std::cerr << ("Could not create render context!\n");
        return nullptr;       
    }

    // set callbacks
    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);

    return renderWindow.release();
}

GraphicsProfile* GlfwWindowManager::createGraphicsProfile(GraphicsAPI api, int major, int minor) {
    if (api == GraphicsAPI::OPENGL)
        return new GlfwGL3Profile();
    if (api == GraphicsAPI::OPENGL_ES)
        return new GlfwGLES2Profile();
    return nullptr;
}

const std::unordered_map<GLFWwindow*, GlfwRenderWindow*>& GlfwWindowManager::getWindows() const {
    return _windows;
}

}
