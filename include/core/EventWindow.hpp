#pragma once

#include "EventMacro.hpp"

namespace fui {

class EventWindow {
public:
  void onKeyEvent(int key, int action, int mods) { _signalKey.emit(key, action, mods); }
  void onMouseButtonEvent(int button, int action, int mods) { _signalMouseButton.emit(button, action, mods); }
  void onMouseMoveEvent(int xpos, int ypos) { _signalMouseMove.emit(xpos, ypos); }
  void onResizeEvent(int width, int height) { _signalResize.emit(width, height); }

  FUI_DEFINE_EVENT(void(int, int, int), Key);
  FUI_DEFINE_EVENT(void(int, int, int), MouseButton);
  FUI_DEFINE_EVENT(void(int, int), MouseMove);
  FUI_DEFINE_EVENT(void(int, int), Resize);
};

} // namespace fui
