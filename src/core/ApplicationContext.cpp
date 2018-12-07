#include "core/ApplicationContext.hpp"
#include "core/RenderWindow.hpp"
#include "core/Status.hpp"

#include <GLFW/glfw3.h>

namespace fui {

ApplicationContext::ApplicationContext()
: window(nullptr)
{
}

ApplicationContext::~ApplicationContext() = default;

void ApplicationContext::run (RenderWindow& window) {
    auto nativeWindow = static_cast<GLFWwindow*>(window.nativeWindow());
    this->window = &window;

    onEnter();
	while (!glfwWindowShouldClose(nativeWindow)) {
        onDraw();
        glfwPollEvents();
    }
    onExit();
}

Status ApplicationContext::onEnter() {
    return Status::OK;
}

}
