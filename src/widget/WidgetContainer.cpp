#include "widget/WidgetContainer.hpp"
#include <cassert>

namespace fui {

WidgetContainer::WidgetContainer(WidgetContainer* parent)
: Widget(parent) {}

WidgetContainer::~WidgetContainer() {
  auto tmp = std::move(_children);
  for (auto&& w : tmp) {
    delete w;
  }
}

void WidgetContainer::addChild(Widget* widget) {
  assert(widget != this);
  _children.push_back(widget);
}

void WidgetContainer::removeChild(Widget* widget) {
  auto itr = std::find(_children.begin(), _children.end(), widget);
  if (itr != _children.end()) {
    _children.erase(itr);
  }
}

void WidgetContainer::draw(RenderContext& renderContext) {
  for (auto&& w : _children) {
    if (w->visible()) {
      w->draw(renderContext);
    }
  }
}

} // namespace fui
