#pragma once

#include "EventWindow.hpp"
#include "widget/WidgetContainer.hpp"

namespace fui {

class RenderContext;

class RenderWindow : public EventWindow, public WidgetContainer {
public:
  RenderWindow();

  virtual ~RenderWindow() = default;

  void drawGui();

  virtual void* nativeWindow() const = 0;

  virtual RenderContext* renderContext() const = 0;

  virtual void getCursorPos(int& x, int& y) = 0;

  virtual void getWindowSize(int& width, int& height) = 0;

  virtual void getDrawableSize(int& width, int& height) = 0;

  virtual void close() = 0;

  virtual void hide() = 0;

  virtual void show() = 0;

  virtual void setSwapInterval(int interval) = 0;

  virtual void swapBuffer() = 0;
};

} // namespace fui
