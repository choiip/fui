#pragma once

#include <vector>
#include "Widget.hpp"

namespace fui {

typedef std::vector<Widget*> WidgetList;

class WidgetContainer : public Widget {
public:
  explicit WidgetContainer(WidgetContainer* parent);

  virtual ~WidgetContainer();

  /// Variadic shorthand notation to construct and add a child widget
  template <typename WidgetClass, typename... Args> WidgetClass* addChild(Args&&... args) {
    return new WidgetClass(this, std::forward<Args>(args)...);
  }

  /// Convenience function which appends a widget at the end
  void addChild(Widget* widget);

  /// Remove a child widget by value
  void removeChild(Widget* widget);

  virtual void draw(RenderContext& renderContext) override;

  virtual Vector2i childrenOrigin() const;
protected:
  virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
  virtual void onMousePressEvent(MouseEvent& event) override;
  virtual void onMouseReleaseEvent(MouseEvent& event) override;

private:
  WidgetList _children;
};

} // namespace fui
