#pragma once

namespace fui {

class RenderContext;

class RenderWindow {
public:
    virtual ~RenderWindow() = default;

    virtual void* nativeWindow() const = 0;
    
    virtual RenderContext* renderContext() const = 0;

    virtual void getCursorPos(int& x, int& y) = 0;

    virtual void getWindowSize(int& width, int& height) = 0;

    virtual void getDrawableSize(int& width, int& height) = 0;

    virtual void setSwapInterval(int interval) = 0;

    virtual void swapBuffer() = 0;
};

}
