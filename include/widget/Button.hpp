#pragma once

#include "Widget.hpp"

namespace fui {

class Button : public Widget {
public:
  /// Flags to specify the button behavior (can be combined with binary OR)
  enum class Flags {
    NormalButton = (1 << 0), ///< A normal Button.
    RadioButton = (1 << 1),  ///< A radio Button.
    ToggleButton = (1 << 2), ///< A toggle Button.
    PopupButton = (1 << 3)   ///< A popup Button.
  };

  /// The available icon positions.
  enum class IconPosition {
    Left,          ///< Button icon on the far left.
    LeftCentered,  ///< Button icon on the left, centered (depends on
                   ///< caption text length).
    RightCentered, ///< Button icon on the right, centered (depends on
                   ///< caption text length).
    Right          ///< Button icon on the far right.
  };

  explicit Button(WidgetContainer* parent, const Text& caption = "Untitled");

  virtual ~Button();

  virtual void draw(RenderContext& renderContext) override;

protected:
  /// The pressed signal for all types of buttons.
  FUI_WIDGET_EVENT(void(), Pressed);

  /// The toggled signal for toggle buttons.
  FUI_WIDGET_EVENT(void(bool), Toggled);

  /// The caption of this Button.
  FUI_WIDGET_PROPERTY(Text, caption, "");

  /**
   * \brief The icon of this Button (``0`` means no icon).
   *
   * \rst
   * The icon to display with this Button.  If not ``0``, may either be a
   * picture icon, or one of the icons enumerated in
   * :ref:`entypo.h`.  The kind of icon (image or Entypo)
   * is determined by the functions :func:`hauwan::nvgIsImageIcon` and its
   * reciprocal counterpart :func:`hauwan::nvgIsFontIcon`.
   * \endrst
   */
  FUI_WIDGET_PROPERTY(int, icon, 0);

  /// The position to draw the icon at.
  FUI_WIDGET_PROPERTY(IconPosition, iconPosition, IconPosition::LeftCentered);

  /// Whether or not this Button is currently pushed.
  FUI_WIDGET_PROPERTY(bool, pushed, false);

  /// The current flags of this button (see \ref hauwan::Button::Flags for options).
  FUI_WIDGET_PROPERTY(Flags, flags, Flags::NormalButton);

  /// The background color of this Button.
  FUI_WIDGET_PROPERTY(Color, backgroundColor, ConstantColor::Black);

  /// The color of the caption text of this Button.
  FUI_WIDGET_PROPERTY(Color, textColor, ConstantColor::Black);
};

} // namespace fui
