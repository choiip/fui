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
    virtual RenderWindow* createWindow(int width, int height) override;

    const std::unordered_map<GLFWwindow*, GlfwRenderWindow*>& getWindows() const;
};

}
