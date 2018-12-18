#pragma once

namespace fui {

class AbstractWindowManager;
enum class Status;

class ApplicationContext {
public:
  ApplicationContext();
  virtual ~ApplicationContext();
  void run(AbstractWindowManager& windowManager);
  void drawOnce();

protected:
  virtual Status onEnter();
  virtual void onDraw();
  virtual void onExit();
};

} // namespace fui
