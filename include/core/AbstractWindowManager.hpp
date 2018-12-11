#pragma once

namespace fui {

class RenderWindow;

class AbstractWindowManager {
public:
    virtual ~AbstractWindowManager() = default;
    virtual RenderWindow* createWindow(int width, int height) = 0;
};

}
