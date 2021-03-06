#pragma once

#include "Widget.hpp"

namespace fui {

class Button : public Widget {
public:
  /// Type of button
  enum class Type {
    NORMAL,
    TOGGLE,
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
  virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
  virtual void onMousePressEvent(MouseEvent& event) override;
  virtual void onMouseReleaseEvent(MouseEvent& event) override;

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
   * :ref:`text/EntypoIconDef.h`.  The kind of icon (image or Entypo)
   * is determined by the functions :func:`nvgIsImageIcon` and its
   * reciprocal counterpart :func:`nvgIsFontIcon`.
   * \endrst
   */
  FUI_WIDGET_PROPERTY(int, icon, 0);

  /// The position to draw the icon at.
  FUI_WIDGET_PROPERTY(IconPosition, iconPosition, IconPosition::LeftCentered);

  /// Whether or not this Button is currently pushed.
  FUI_WIDGET_PROPERTY(bool, pushed, false);

  /// The type of this button (see \ref Button::Type for options).
  FUI_WIDGET_PROPERTY(Type, type, Type::NORMAL);

  /// The background color of this Button.
  FUI_WIDGET_PROPERTY(Color, backgroundColor, {0, 0, 0, 0});

  /// The color of the caption text of this Button.
  FUI_WIDGET_PROPERTY(Color, textColor, {0, 0, 0, 0});

private:
  struct SnapState;
  std::unique_ptr<SnapState> _snap;
};

} // namespace fui
