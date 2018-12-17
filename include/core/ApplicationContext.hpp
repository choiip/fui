#pragma once

namespace fui {

class RenderWindow;
enum class Status;

class ApplicationContext {
public:
  ApplicationContext();
  virtual ~ApplicationContext();
  void run(RenderWindow& window);
  void drawOnce();

protected:
  virtual Status onEnter();
  virtual void onDraw();
  virtual void onExit();

  RenderWindow* window;
};

} // namespace fui
