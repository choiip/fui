#include "widget/Widget.hpp"
#include "widget/WidgetContainer.hpp"

namespace fui {

Widget::Widget(WidgetContainer* parent)
: _parent(parent) {
  if (_parent) {
    _parent->addChild(this);
  }
}

Widget::~Widget() {
  if (_parent) {
    _parent->removeChild(this);
  }
}

void Widget::onMouseMoveEvent(MouseMoveEvent& event) {}
void Widget::onMousePressEvent(MouseEvent& event) {}
void Widget::onMouseReleaseEvent(MouseEvent& event) {}
void Widget::draw(RenderContext& renderContext) {}

bool Widget::contain(int x, int y) const {
  auto dx = x - _position.x;
  auto dy = y - _position.y;
  return dx >= 0 && dx < _size.x && dy >= 0 && dy < _size.y;
}

const WidgetStyle& Widget::style() const { return (_style ? *(const WidgetStyle*)_style.get() : _parent->style()); }

} // namespace fui
