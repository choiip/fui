#pragma once

#include "core/AbstractWindowManager.hpp"
#include "core/Singleton.hpp"
#include <list>
#include <unordered_map>

struct GLFWwindow;

namespace fui {

class GlfwRenderWindow;

class GlfwWindowManager
: public AbstractWindowManager
, public Singleton<GlfwWindowManager> {
private:
  std::unordered_map<GLFWwindow*, GlfwRenderWindow*> _windows;
  std::list<RenderWindow*>  _disposedWindows;

public:
  GlfwWindowManager();
  virtual ~GlfwWindowManager();
  virtual RenderWindow* createWindow(int width, int height, const GraphicsProfile& graphicsProfile) override;
  virtual GraphicsProfile* createGraphicsProfile(GraphicsAPI api, int major, int minor) override;
  virtual void pollEvent() override;
  virtual bool shouldQuit() override;

  auto getWindows() const -> const decltype(_windows)&;
};

} // namespace fui
