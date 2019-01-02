#pragma once

#include "core/MathDef.hpp"

namespace fui {

class RenderContext;

class WidgetStyle {
public:
  explicit WidgetStyle(RenderContext& renderContext);

  /// Fonts ///
  /// The icon font face
  int fontIcons = -1;
  /// The standard font face
  int fontStandard = -1;
  /// The bold font face
  int fontBold = -1;

  /// The font size for all widgets
  float standardFontSize = 16.f;
  float buttonFontSize = 20.f;

  /// The border color
  /**
   * The dark border color.
   */
  Color borderDark = {.11f, .11f, .11f, 1.f};
  /**
   * The medium border color.
   */
  Color borderMedium = {.14f, .14f, .14f, 1.f};
  /**
   * The light border color.
   */
  Color borderLight = {.36f, .36f, .36f, 1.f};

  /// Text
  /// The text color
  Color standardTextColor = {1.f, 1.f, 1.f, .63f};
  /// The disabled text color
  Color disabledTextColor = {1.f, 1.f, 1.f, .31f};
  /// The text Shadow
  Color textShadow = {0.f, 0.f, 0.f, .63f};

  /// Window ///
  /// Rounding radius for Window widget corners.
  float windowCornerRadius = 2.f;
  /// Default size of Window widget titles.
  int windowHeaderHeight = 30;
  /// Size of drop shadow rendered behind the Window widgets.
  int windowDropShadowSize = 10;
  /**
   * The fill color for a Window that is not in focus.
   */
  Color windowFillNormal = {.17f, .17f, .17f, .9f};
  /**
   * The fill color for a Window that is in focus.
   */
  Color windowFillFocused = {.18f, .18f, .18f, .9f};
  /**
   * The title color for a Window that is not in focus.
   */
  Color windowTitleNormal = {.86f, .86f, .86f, .63f};
  /**
   * The title color for a Window that is in focus.
   */
  Color windowTitleFocused = {1.f, 1.f, 1.f, .74f};
  /**
   * The top gradient color for Window headings.
   */
  Color windowHeaderGradientTop = {.29f, .29f, .29f, .91f};
  /**
   * The bottom gradient color for Window headings.
   */
  Color windowHeaderGradientBot = {.23f, .23f, .23f, .91f};
  /// The Window header top separation color.
  Color windowHeaderTopSep = {.36f, .36f, .36f, 1.f};
  /// The Window header bottom separation color.
  Color windowHeaderBotSep = {.11f, .11f, .11f, 1.f};
  /**
   * The popup window color.
   */
  Color windowPopup = {.19f, .19f, .19f, 1.f};
  /**
   * The transparent popup window color.
   */
  Color windowPopupTransparent = {.19f, .19f, .19f, .0f};

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
  Color buttonGradientTopPushed = {.16f, .16f, .16f, .8f};
  /**
   * The bottom gradient color for buttons currently pushed.
   */
  Color buttonGradientBotPushed = {.11f, .11f, .11f, .8f};
};

} // namespace fui
