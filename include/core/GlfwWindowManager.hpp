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
  virtual void pollEvent() override;
  virtual bool shouldQuit() override;

  bool hasRunableWindow() const;
  auto getWindows() const -> const decltype(_windows)&;
};

} // namespace fui
