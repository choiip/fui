#pragma once

#include <vector>
#include "Widget.hpp"

namespace fui {

typedef std::vector<Widget*> WidgetList;

class WidgetContainer : public Widget {
public:
  WidgetContainer(WidgetContainer* parent);

  virtual ~WidgetContainer();

  /// Variadic shorthand notation to construct and add a child widget
  template <typename WidgetClass, typename... Args> WidgetClass* addChild(const Args&... args) {
    return new WidgetClass(this, args...);
  }

  /// Convenience function which appends a widget at the end
  void addChild(Widget* widget);

  /// Remove a child widget by value
  void removeChild(Widget* widget);

  virtual void draw(RenderContext& renderContext) override;

private:
  WidgetList _children;
};

} // namespace fui
