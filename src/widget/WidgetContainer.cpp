#include "widget/WidgetContainer.hpp"
#include <algorithm>
#include <cassert>
#include "core/RenderContext.hpp"
#include "event/FocusEvent.hpp"
#include "event/MouseEvent.hpp"
#include "nanovg/nanovg.h"

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
  if (widget == nullptr) return;
  assert(widget != this);
  assert(widget->_parent == nullptr);
  widget->_parent = this;
  _children.push_back(widget);
}

void WidgetContainer::removeChild(Widget* widget) {
  if (widget->_parent != this) return;
  _children.erase(std::remove(std::begin(_children), std::end(_children), widget), _children.end());
  widget->_parent = nullptr;
}

void WidgetContainer::draw(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  auto origin = _position + childrenOrigin();

  nvgSave(vg);
  nvgTranslate(vg, origin.x, origin.y);
  drawChildren(renderContext);
  nvgRestore(vg);
}

Vector2i WidgetContainer::childrenOrigin() const {
  return Vector2i::Zero;
}

void WidgetContainer::drawChildren(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  for (auto&& w : _children) {
    if (w->visible()) {
      nvgSave(vg);
      nvgIntersectScissor(vg, w->position().x, w->position().y, w->size().x, w->size().y);
      w->draw(renderContext);
      nvgRestore(vg);
    }
  }
}  

void WidgetContainer::onChildFocusChangeEvent(FocusEvent& event) {
  if (event.value == Focus::IN) {
    const Widget* lastChild = _children.back();
    if (lastChild != event.source) {
      if (lastChild->focused()) {
        _children.back()->focused(false);
      }
      auto itr = std::find(_children.begin(), _children.end(), event.source);
      _children.erase(itr);
      _children.push_back(event.source);
    }
    focused(true);
  }
}

void WidgetContainer::onFocusChangeEvent(FocusEvent& event) {
  if (!_children.empty()) {
    _children.back()->focused(event.value == Focus::IN);
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
      auto movement =
          prevContained == contained ? Movement::MOVING : (contained ? Movement::ENTERING : Movement::LEAVING);
      MouseMoveEvent altEvent({localPrevX, localPrevY}, movement, {localX, localY}, event.button, event.buttons,
                              event.modifiers);
      w->onMouseMoveEvent(altEvent);
    }
  }
}

void WidgetContainer::onMousePressEvent(MouseEvent& event) {
  auto local = event.position - _position;
  auto found = std::find_if(std::rbegin(_children),
                            std::rend(_children),
                            [local](const Widget* w){
                              return w->visible() && w->enabled() && w->contain(local.x, local.y);
                            });
  if (found != std::rend(_children)) {
    auto hitChild = *found;
    hitChild->focused(true);
    MouseEvent altEvent = {local, event.button, event.buttons, event.modifiers};
    hitChild->onMousePressEvent(altEvent);
  }
}

void WidgetContainer::onMouseReleaseEvent(MouseEvent& event) {
  auto local = event.position - _position;
  MouseEvent altEvent = {local, event.button, event.buttons, event.modifiers};
  for (auto&& w : _children) {
    if (w->visible() && w->enabled()) {
      w->onMouseReleaseEvent(altEvent);
    }
  }
}

} // namespace fui
