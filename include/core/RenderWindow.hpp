#pragma once

namespace fui {

class RenderWindow {
public:
    virtual ~RenderWindow() = default;

    virtual void* nativeWindow() = 0;
    
    virtual void getCursorPos(int& x, int& y) = 0;

    virtual void getWindowSize(int& width, int& height) = 0;

    virtual void getDrawableSize(int& width, int& height) = 0;

    virtual void setSwapInterval(int interval) = 0;

    virtual void swapBuffer() = 0;
};

}
