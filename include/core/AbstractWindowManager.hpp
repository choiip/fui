#pragma once

namespace fui {

class RenderWindow;
class GraphicsProfile;
enum class GraphicsAPI;

class AbstractWindowManager {
public:
  virtual ~AbstractWindowManager() = default;
  virtual RenderWindow* createWindow(int width, int height, const GraphicsProfile& graphicsProfile) = 0;
  virtual GraphicsProfile* createGraphicsProfile(GraphicsAPI api, int major, int minor) = 0;
  virtual void pollEvent() = 0;
  virtual bool shouldQuit() = 0;
};

} // namespace fui
