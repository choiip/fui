#pragma once

#include "AbstractWindowManager.hpp"
#include "Singleton.hpp"
#include <unordered_map>

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

} // namespace fui
