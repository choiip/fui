#pragma once

#include <memory>
#include <string>
#include "core/MathDef.hpp"
#include "WidgetMacro.hpp"

namespace fui {

typedef std::string Text;

class WidgetContainer;
class WidgetStyle;
class RenderContext;
class FocusEvent;
class MouseEvent;
class MouseMoveEvent;

class Widget {
public:
  explicit Widget(WidgetContainer* parent);

  virtual ~Widget();

  WidgetContainer* parent() const { return _parent; }

  virtual void draw(RenderContext& renderContext);
  
protected:
  /// Check if the widget contains a certain position
  bool contain(int x, int y) const;

  /// Translate the widget coordinate rectangle to desired coordinate system
  Recti mapTo(const Recti& rect, Coordinate coord) const;

  /// Return the \ref WidgetStyle used to draw this widget
  const WidgetStyle& style() const;

  virtual void onFocusChangeEvent(FocusEvent& event);
  virtual void onMouseMoveEvent(MouseMoveEvent& event);
  virtual void onMousePressEvent(MouseEvent& event);
  virtual void onMouseReleaseEvent(MouseEvent& event);

  /// The position relative to the parent widget.
  FUI_WIDGET_PROPERTY(Vector2i, position, {0, 0});

  /// The size of the widget.
  FUI_WIDGET_PROPERTY(Vector2i, size, {0, 0});

  /**
   * \brief Set the fixed size of this widget
   *
   * If nonzero, components of the fixed size attribute override any values
   * computed by a layout generator associated with this widget. Note that
   * just setting the fixed size alone is not enough to actually change its
   * size; this is done with a call to \ref setSize or a call to \ref
   * performLayout() in the parent widget.
   */
  FUI_WIDGET_PROPERTY(Vector2i, fixedSize, {0, 0});

  /**
   * Whether or not this Widget is currently visible.  When a Widget is not
   * currently visible, no time is wasted executing its drawing method.
   */
  FUI_WIDGET_PROPERTY(bool, visible, true);

  /**
   * Whether or not this Widget is currently enabled.  Various different kinds
   * of derived types use this to determine whether or not user input will be
   * accepted.  For example, when ``mEnabled == false``, the state of a
   * CheckBox cannot be changed, or a TextBox will not allow new input.
   */
  FUI_WIDGET_PROPERTY(bool, enabled, true);

  /// Whether or not this widget is currently focused
  FUI_WIDGET_PROPERTY_ALT_SETTER(bool, focused, false);

  /// Widget style
  FUI_WIDGET_WRITEONLY_PROPERTY(std::shared_ptr<WidgetStyle>, style, nullptr);

private:
  auto _focusedSetter(bool const& v) -> decltype(this);

  virtual Recti regionAtFrameBuffer(const Recti& rect) const;

  WidgetContainer* _parent;

  friend class WidgetContainer;
};

} // namespace fui
