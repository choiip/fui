#include "widget/WidgetContainer.hpp"
#include <cassert>
#include "event/MouseEvent.hpp"

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

void WidgetContainer::onMouseMoveEvent(MouseMoveEvent& event) {
  auto localX = event.position.x - _position.x;
  auto localY = event.position.y - _position.y;
  auto localPrevX = event.prevPosition.x - _position.x;
  auto localPrevY = event.prevPosition.y - _position.y;

  for (auto&& w : _children) {
    if (w->visible() && w->enabled()) {
      auto prevContained = w->contain(localPrevX, localPrevY);
      auto contained = w->contain(localX, localY);
      if (prevContained || contained) {
        auto movement =
            prevContained == contained ? Movement::MOVING : (contained ? Movement::ENTERING : Movement::LEAVING);
        MouseMoveEvent altEvent({localPrevX, localPrevY}, movement, {localX, localY}, event.button, event.buttons,
                                event.modifiers);
        w->onMouseMoveEvent(altEvent);
      }
    }
  }
}

void WidgetContainer::onMousePressEvent(MouseEvent& event) {
  auto localX = event.position.x - _position.x;
  auto localY = event.position.y - _position.y;
  MouseEvent altEvent = {{localX, localY}, event.button, event.buttons, event.modifiers};
  for (auto&& w : _children) {
    if (w->visible() && w->enabled() && w->contain(localX, localY)) {
      w->onMousePressEvent(altEvent);
    }
  }
}

void WidgetContainer::onMouseReleaseEvent(MouseEvent& event) {
  auto localX = event.position.x - _position.x;
  auto localY = event.position.y - _position.y;
  MouseEvent altEvent = {{localX, localY}, event.button, event.buttons, event.modifiers};
  for (auto&& w : _children) {
    if (w->visible() && w->enabled() && w->contain(localX, localY)) {
      w->onMouseReleaseEvent(altEvent);
    }
  }
}

} // namespace fui
