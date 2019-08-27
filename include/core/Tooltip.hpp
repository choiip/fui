#pragma once

#include "widget/WidgetMacro.hpp"
#include "MathDef.hpp"

namespace fui {

class RenderContext;

class Tooltip {
public:
  /// Type of bubble
  enum class Bubble {
    TOP,
    BOTTOM,
  };

  void draw(RenderContext& renderContext);

protected:
  /// The position relative to the render window.
  FUI_WIDGET_PROPERTY(Vector2i, absolutePosition, {0, 0});

  /// The tooltip string
  FUI_WIDGET_PROPERTY(std::string, string, "");

  /// The tooltip maximum width
  FUI_WIDGET_PROPERTY(int, width, 150);

  /// The tooltip bubble appearance
  FUI_WIDGET_PROPERTY(Bubble, bubble, Bubble::TOP);

  /// The tooltip font
  FUI_WIDGET_PROPERTY(int, font, -1);

  /// The tooltip font size
  FUI_WIDGET_PROPERTY(float, fontSize, 15.f);
};

} // namespace fui
