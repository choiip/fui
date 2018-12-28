#pragma once

#include "core/MathDef.hpp"

struct NVGcontext;

namespace fui {

class WidgetStyle {
public:
  explicit WidgetStyle(NVGcontext* vg);

  /// Fonts ///
  /// The icon font face 
  int fontIcons;
  /// The standard font face
  int fontStandard;
  /// The bold font face
  int fontBold;

  /// The font size for all widgets
  float standardFontSize = 16.f;
  float buttonFontSize = 20.f;

  /// Text
  /// The text color
  Color standardTextColor = { 1.f, 1.f, 1.f, .63f };
  /// The disabled text color
  Color disabledTextColor = { 1.f, 1.f, 1.f, .31f };
  /// The text Shadow
  Color textShadow = { 0.f, 0.f, 0.f, .63f };

  /// Button ///
  /// Rounding radius for Button (and derived types) widgets.
  float buttonCornerRadius = 4.f;
  /**
   * The top gradient color for normal buttons.
   */
  Color buttonGradientTopNormal = {.29f, .29f, .29f, .1f};
  /**
   * The bottom gradient color for normal buttons.
   */
  Color buttonGradientBotNormal = {.23f, .23f, .23f, .1f};
  /**
   * The top gradient color for buttons in focus.
   */
  Color buttonGradientTopFocused = {.25f, .25f, .25f, .5f};
  /**
   * The bottom gradient color for buttons in focus.
   */
  Color buttonGradientBotFocused = {.19f, .19f, .19f, .5f};
  /**
   * The top gradient color for buttons currently pushed.
   */
  Color buttonGradientTopPushed = {.16f, .16f, .16f, 0.8f};
  /**
   * The bottom gradient color for buttons currently pushed.
   */
  Color buttonGradientBotPushed = {.11f, .11f, .11f, 0.8f};

private:
  NVGcontext* _vg;
};

} // namespace fui
