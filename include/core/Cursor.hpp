#pragma once

#include "core/MathDef.hpp"

namespace fui {

class Cursor {
public:
  enum class Shape {
    ARROW = 0,
    IBEAM,
    WAIT,
    CROSSHAIR,
    WAITARROW,
    SIZENWSE,
    SIZENESW,
    SIZEWE,
    SIZENS,
    SIZEALL,
    NO,
    HAND,
  };

  virtual ~Cursor() {}

  virtual void hide() = 0;

  virtual void show() = 0;

  virtual Vector2i position() = 0;

  virtual void setShape(Shape shape) = 0;
};

} // namespace fui
