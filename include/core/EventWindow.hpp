#pragma once

#include "EventMacro.hpp"

namespace fui {

class EventWindow {
public:
  void onMouseMoveEvent(int xpos, int ypos) { _signalMouseMove.emit(xpos, ypos); }

  DEFINE_EVENT(void(int, int), MouseMove);
};

} // namespace fui
