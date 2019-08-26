#pragma once

#include "core/MathDef.hpp"
#include "event/EventEnum.hpp"

namespace fui {

class MouseEvent {
public:
  const Vector2i position;
  const MouseButton button;
  const MouseButton buttons;
  const Modifier modifiers;

  MouseEvent(const Vector2i& position, MouseButton button, MouseButton buttons, Modifier modifiers)
  : position(position)
  , button(button)
  , buttons(buttons)
  , modifiers(modifiers) {}
};

enum class Movement { ENTERING, MOVING, LEAVING };

class MouseMoveEvent {
public:
  const Vector2i prevPosition;
  const Movement movement;
  const Vector2i position;
  const MouseButton buttons;
  const Modifier modifiers;

  MouseMoveEvent(const Vector2i& prevPosition, Movement movement, const Vector2i& position,
                 MouseButton buttons, Modifier modifiers)
  : prevPosition(prevPosition)
  , movement(movement)
  , position(position)
  , buttons(buttons)
  , modifiers(modifiers) {}
};

inline std::ostream& operator<<(std::ostream& os, const Modifier& v) {
  return os << (int)v;
}
inline std::ostream& operator<<(std::ostream& os, const MouseButton& v) {
  return os << (int)v;
}
inline std::ostream& operator<<(std::ostream& os, const MouseEvent& v) {
  return os << v.position << ',' << v.button << ',' << v.buttons << ',' << v.modifiers;
}
inline std::ostream& operator<<(std::ostream& os, const Movement& v) {
  return os << (int)v;
}
inline std::ostream& operator<<(std::ostream& os, const fui::MouseMoveEvent& v) {
  return os << v.prevPosition << ',' 
            << v.movement << ',' 
            << v.position << ','
            << v.buttons << ','
            << v.modifiers;
}

} // namespace fui

