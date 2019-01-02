#pragma once

#include "core/RenderWindow.hpp"
#include "core/GlfwCursor.hpp"

struct GLFWwindow;

namespace fui {

class GlfwRenderWindow : public RenderWindow {
public:
  GlfwRenderWindow(GLFWwindow* window, RenderContext* renderContext);

  virtual ~GlfwRenderWindow();

  virtual void* nativeWindow() const override;

  virtual Cursor* cursor() override;

  virtual void getWindowSize(int& width, int& height) override;

  virtual void getDrawableSize(int& width, int& height) override;

  virtual void close() override;

  virtual void hide() override;

  virtual void show() override;

  virtual void setSwapInterval(int interval) override;

  virtual void swapBuffer() override;

private:
  GLFWwindow* _window;
  GlfwCursor _cursor;
};

} // namespace fui
