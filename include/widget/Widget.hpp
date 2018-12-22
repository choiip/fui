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

class Widget {
public:
  explicit Widget(WidgetContainer* parent);

  virtual ~Widget();

  WidgetContainer* parent() const { return _parent; }

  void setStyle(const std::shared_ptr<WidgetStyle>& style);
  
  virtual void draw(RenderContext& renderContext);

protected:
  const WidgetStyle& style() const;

  /// The position relative to the parent widget.
  FUI_WIDGET_PROPERTY(Vector2i, position, ConstantVector2i::Zero);

  /// The size of the widget.
  FUI_WIDGET_PROPERTY(Vector2i, size, ConstantVector2i::Zero);

  /**
   * \brief Set the fixed size of this widget
   *
   * If nonzero, components of the fixed size attribute override any values
   * computed by a layout generator associated with this widget. Note that
   * just setting the fixed size alone is not enough to actually change its
   * size; this is done with a call to \ref setSize or a call to \ref
   * performLayout() in the parent widget.
   */
  FUI_WIDGET_PROPERTY(Vector2i, fixedSize, ConstantVector2i::Zero);

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
  FUI_WIDGET_PROPERTY(bool, focused, false);

private:
  WidgetContainer* _parent;
  std::shared_ptr<WidgetStyle> _style;

};

} // namespace fui
