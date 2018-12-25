#pragma once

#include "core/MathDef.hpp"

struct NVGcontext;

namespace fui {

class WidgetStyle {
public:
  WidgetStyle(NVGcontext* vg);

  /// Fonts ///
  int fontIcons;
  int fontNormal;
  int fontBold;

  /// Button ///
  /// Rounding radius for Button (and derived types) widgets.
  float buttonCornerRadius = 4.f;
  /**
   * The top gradient color for normal buttons.
   */
  Color buttonGradientTopNormal = {.29f, .29f, .29f, 1.f};
  /**
   * The bottom gradient color for normal buttons.
   */
  Color buttonGradientBotNormal = {.23f, .23f, .23f, 1.f};
  /**
   * The top gradient color for buttons in focus.
   */
  Color buttonGradientTopFocused = {.25f, .25f, .25f, 1.f};
  /**
   * The bottom gradient color for buttons in focus.
   */
  Color buttonGradientBotFocused = {.19f, .19f, .19f, 1.f};
  /**
   * The top gradient color for buttons currently pushed.
   */
  Color buttonGradientTopPushed = {.16f, .16f, .16f, 1.f};
  /**
   * The bottom gradient color for buttons currently pushed.
   */
  Color buttonGradientBotPushed = {.11f, .11f, .11f, 1.f};

private:
  NVGcontext* _vg;
};

} // namespace fui
