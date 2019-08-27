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

protected:
  virtual Vector2i childrenOrigin() const;
  virtual Widget* findWidget(const Vector2i& point, int recursiveLevel = -1) override;
  void drawChildren(RenderContext& renderContext);
  void onChildFocusChangeEvent(FocusEvent& event);
  
  virtual void onFocusChangeEvent(FocusEvent& event) override;
  virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
  virtual void onMousePressEvent(MouseEvent& event) override;
  virtual void onMouseReleaseEvent(MouseEvent& event) override;
  
private:
  WidgetList _children;

  friend class Widget;
};

} // namespace fui
