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
};

} // namespace fui
