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

void Widget::setStyle(const std::shared_ptr<WidgetStyle>& style) {
  _style = style;
}

void Widget::draw(RenderContext& renderContext) {}

const WidgetStyle& Widget::style() const {
  return (_style ? *(const WidgetStyle*)_style.get() : _parent->style());
}

} // namespace fui
