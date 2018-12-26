#include "core/RenderWindow.hpp"
#include "core/Cursor.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"
#include "event/MouseEvent.hpp"
#include "widget/WidgetStyle.hpp"

namespace fui {

RenderWindow::RenderWindow()
: WidgetContainer(nullptr) {}

void RenderWindow::drawGui() {
  auto ctx = renderContext();

  if (ctx && _visible) {
    auto vg = ctx->vg();
    // load default sytle
    static auto defaultStyle = std::make_shared<WidgetStyle>(vg);
    Widget::setStyle(defaultStyle);

    int winWidth, winHeight, fbWidth, fbHeight;
    getWindowSize(winWidth, winHeight);
    getDrawableSize(fbWidth, fbHeight);
    // Calculate pixel ration for hi-dpi devices.
    auto pxRatio = (float)fbWidth / (float)winWidth;

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
    WidgetContainer::draw(*ctx);
    nvgEndFrame(vg);
  }
}

void RenderWindow::onKeyEvent(Key key, ButtonAction action, Modifier mods) { _signalKey.emit(key, action, mods); }
void RenderWindow::onMouseButtonEvent(MouseButton button, ButtonAction action, Modifier mods) {
  if (action == ButtonAction::RELEASE) {
    _buttonInPressing = _buttonInPressing & ~button;
  } else {
    _buttonInPressing = _buttonInPressing | button;
  }
  MouseEvent event = {cursor()->position(), button, _buttonInPressing, Modifier::NONE};

  if (action == ButtonAction::RELEASE) {
    WidgetContainer::onMouseReleaseEvent(event);
  } else if (action == ButtonAction::PRESS) {
    WidgetContainer::onMousePressEvent(event);
  }

  _signalMouseButton.emit(button, action, mods);
}
void RenderWindow::onMouseMoveEvent(int xpos, int ypos) {
  MouseEvent event = {{xpos, ypos}, MouseButton::NONE, _buttonInPressing, Modifier::NONE};
  WidgetContainer::onMouseMoveEvent(event);
  _signalMouseMove.emit(xpos, ypos);
}
void RenderWindow::onResizeEvent(int width, int height) { _signalResize.emit(width, height); }

} // namespace fui
