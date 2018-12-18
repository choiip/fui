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

void Widget::draw(RenderContext& renderContext) {}

} // namespace fui
