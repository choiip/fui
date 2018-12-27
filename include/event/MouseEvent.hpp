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

class MouseMoveEvent : public MouseEvent {
public:
  const Vector2i prevPosition;
  const Movement movement;

  MouseMoveEvent(const Vector2i& prevPosition, Movement movement, const Vector2i& position, MouseButton button,
                 MouseButton buttons, Modifier modifiers)
  : MouseEvent(position, button, buttons, modifiers)
  , prevPosition(prevPosition)
  , movement(movement) {}
};

} // namespace fui
