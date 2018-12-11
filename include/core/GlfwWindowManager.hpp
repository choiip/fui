#pragma once

#include <unordered_map>
#include "AbstractWindowManager.hpp"
#include "Singleton.hpp"

struct GLFWwindow;

namespace fui {

class GlfwRenderWindow;

class GlfwWindowManager 
: public AbstractWindowManager
, public Singleton<GlfwWindowManager> {
private:
    std::unordered_map<GLFWwindow*, GlfwRenderWindow*> _windows;

public:
    GlfwWindowManager();
    virtual ~GlfwWindowManager();
    virtual RenderWindow* createWindow(int width, int height, const GraphicsProfile& graphicsProfile) override;
    virtual GraphicsProfile* createGraphicsProfile(GraphicsAPI api, int major, int minor) override;

    const std::unordered_map<GLFWwindow*, GlfwRenderWindow*>& getWindows() const;
};

}
