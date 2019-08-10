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

Recti Widget::mapTo(const Recti& rect, Coordinate coord) const {
  if (_parent == nullptr) {
    switch (coord) {
      case Coordinate::Framebuffer: return regionAtFrameBuffer(rect);
      case Coordinate::Parent:
      case Coordinate::TopParent: return rect;
      case Coordinate::Screen: 
        return {  _position.x + rect.position.x, 
                  _position.y + rect.position.y,
                  rect.size.x,
                  rect.size.y };  // this widget should be RenderWindow
    }
  }
  auto origin = _parent->childrenOrigin();
  Recti outRect = { origin.x + _position.x + rect.position.x, 
                    origin.y + _position.y + rect.position.y,
                    rect.size.x,
                    rect.size.y };
  if (coord == Coordinate::Parent) {
    return outRect;
  }
  return _parent->mapTo(outRect, coord);
}

const WidgetStyle& Widget::style() const { return (_style ? *(const WidgetStyle*)_style.get() : _parent->style()); }

Recti Widget::regionAtFrameBuffer(const Recti& rect) const {
  if (_parent == nullptr) {
    assert(false);
    return rect;
  }
  return _parent->regionAtFrameBuffer(rect);
}

} // namespace fui
