#pragma once

namespace fui {

enum class Status;

class RenderWindow {
public:
    virtual ~RenderWindow() = default;

    virtual Status create(int width, int height) = 0;

    virtual void destroy() = 0;

    virtual void* nativeWindow() = 0;
    
    virtual void getCursorPos(int& x, int& y) = 0;

    virtual void getWindowSize(int& width, int& height) = 0;

    virtual void getDrawableSize(int& width, int& height) = 0;

    virtual void setSwapInterval(int interval) = 0;

    virtual void swapBuffer() = 0;
};

}
