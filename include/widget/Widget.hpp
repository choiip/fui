#pragma once

#include <string>
#include "core/MathDef.hpp"
#include "WidgetMacro.hpp"

namespace fui {

typedef std::string Text;

class WidgetContainer;
class RenderContext;

class Widget {
public:
  explicit Widget(WidgetContainer* parent);

  virtual ~Widget();

  WidgetContainer* parent() { return _parent; }

  virtual void draw(RenderContext& renderContext);

protected:
  /// The position relative to the parent widget.
  FUI_WIDGET_PROPERTY(Vector2i, position);

  /// The size of the widget.
  FUI_WIDGET_PROPERTY(Vector2i, size);

  /**
   * \brief Set the fixed size of this widget
   *
   * If nonzero, components of the fixed size attribute override any values
   * computed by a layout generator associated with this widget. Note that
   * just setting the fixed size alone is not enough to actually change its
   * size; this is done with a call to \ref setSize or a call to \ref
   * performLayout() in the parent widget.
   */
  FUI_WIDGET_PROPERTY(Vector2i, fixedSize);

  /**
   * Whether or not this Widget is currently visible.  When a Widget is not
   * currently visible, no time is wasted executing its drawing method.
   */
  FUI_WIDGET_PROPERTY(bool, visible);

  /**
   * Whether or not this Widget is currently enabled.  Various different kinds
   * of derived types use this to determine whether or not user input will be
   * accepted.  For example, when ``mEnabled == false``, the state of a
   * CheckBox cannot be changed, or a TextBox will not allow new input.
   */
  FUI_WIDGET_PROPERTY(bool, enabled);

  /// Whether or not this widget is currently focused
  FUI_WIDGET_PROPERTY(bool, focused);

private:
  WidgetContainer* _parent;
};

} // namespace fui
