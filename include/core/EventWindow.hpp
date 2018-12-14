#pragma once

#include "EventMacro.hpp"

namespace fui {

class EventWindow {
public:
  void onKeyEvent(int key, int action, int mods) { _signalKey.emit(key, action, mods); }
  void onMouseMoveEvent(int xpos, int ypos) { _signalMouseMove.emit(xpos, ypos); }

  FUI_DEFINE_EVENT(void(int, int, int), Key);
  FUI_DEFINE_EVENT(void(int, int), MouseMove);
};

} // namespace fui
