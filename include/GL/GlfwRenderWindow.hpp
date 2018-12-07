#pragma once

#include "core/RenderWindow.hpp"

struct GLFWwindow;

namespace fui {

class GlfwRenderWindow : public RenderWindow {
public:
    virtual ~GlfwRenderWindow();

    virtual Status create(int width, int height) override;

    virtual void destroy() override;

    virtual void* nativeWindow() override;

    virtual void getCursorPos(int& x, int& y) override;

    virtual void getWindowSize(int& width, int& height) override;

    virtual void getDrawableSize(int& width, int& height) override;

    virtual void setSwapInterval(int interval) override;

    virtual void swapBuffer() override;

private:
	GLFWwindow* _window;
};

}
