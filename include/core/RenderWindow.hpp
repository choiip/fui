#pragma once

#include "widget/WidgetContainer.hpp"
#include "event/EventEnum.hpp"

namespace fui {

class Cursor;
class RenderContext;

class RenderWindow : public WidgetContainer {
public:
  RenderWindow();

  virtual ~RenderWindow() = default;

  void drawGui();

  void onKeyEvent(Key key, ButtonAction action, Modifier mods);
  void onMouseButtonEvent(MouseButton button, ButtonAction action, Modifier mods);
  void onMouseMoveEvent(int xpos, int ypos);
  void onResizeEvent(int width, int height);

  virtual void* nativeWindow() const = 0;

  virtual Cursor* cursor() = 0;

  virtual RenderContext* renderContext() const = 0;

  virtual void getWindowSize(int& width, int& height) = 0;

  virtual void getDrawableSize(int& width, int& height) = 0;

  virtual void close() = 0;

  virtual void hide() = 0;

  virtual void show() = 0;

  virtual void setSwapInterval(int interval) = 0;

  virtual void swapBuffer() = 0;

  FUI_DEFINE_EVENT(void(Key, ButtonAction, Modifier), Key);
  FUI_DEFINE_EVENT(void(MouseButton, ButtonAction, Modifier), MouseButton);
  FUI_DEFINE_EVENT(void(int, int), MouseMove);
  FUI_DEFINE_EVENT(void(int, int), Resize);

private:
  MouseButton _buttonInPressing;
  Modifier _modifierInPressing;
};

} // namespace fui
