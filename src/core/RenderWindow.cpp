#include "core/RenderWindow.hpp"
#include "core/Cursor.hpp"
#include "core/PerfGraph.hpp"
#include "core/RenderContext.hpp"
#include "core/Tooltip.hpp"
#include "event/FocusEvent.hpp"
#include "event/MouseEvent.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

namespace fui {

RenderWindow::RenderWindow(RenderContext* renderContext)
: WidgetContainer(nullptr)
, _renderContext(renderContext)
, _buttonInPressing(MouseButton::NONE)
, _modifierInPressing(Modifier::NONE)
, _prevCursorPosition({0, 0}) {
  if (renderContext) {
    // load default sytle
    static auto defaultStyle = std::make_shared<WidgetStyle>(*renderContext);
    Widget::style(defaultStyle);
  }
  _tooltip.reset(new Tooltip);
}

RenderWindow::~RenderWindow() { delete _renderContext; }

void RenderWindow::drawGui() {
  if (_renderContext && _visible) {
    auto vg = _renderContext->vg();

    auto viewport = regionAtFrameBuffer({_position.x, _position.y, _size.x, _size.y});

    // Calculate pixel ratio for hi-dpi devices.
    int fbWidth, fbHeight;
    getDrawableSize(fbWidth, fbHeight);
    auto pixelRatio = (float)fbWidth / (float)_size.x;

    nvgBeginFrame(vg, _size.x, _size.y, pixelRatio);
    WidgetContainer::drawChildren(*_renderContext);

    auto hover = WidgetContainer::findWidget(_prevCursorPosition);
    if (hover != nullptr) {
      hover->prepareTooltip(*_tooltip);
      _tooltip->fontSize(20.f)->draw(*_renderContext);
    }
    if (_perfGraph) _perfGraph->draw(*_renderContext);
    _renderContext->setViewport(viewport.x, viewport.y, viewport.w, viewport.h);
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
  MouseEvent event = {cursor()->position(), button, _buttonInPressing, mods};

  if (action == ButtonAction::RELEASE) {
    WidgetContainer::onMouseReleaseEvent(event);
  } else if (action == ButtonAction::PRESS) {
    WidgetContainer::onMousePressEvent(event);
  }

  _signalMouseButton.emit(button, action, mods);
}
void RenderWindow::onMouseMoveEvent(int xpos, int ypos) {
  Vector2i localPos = {xpos, ypos};
  MouseMoveEvent event(_prevCursorPosition, Movement::MOVING, localPos, _buttonInPressing, Modifier::NONE);

  WidgetContainer::onMouseMoveEvent(event);
  _signalMouseMove.emit(localPos.x, localPos.y);
  _prevCursorPosition = localPos;
}
void RenderWindow::onResizeEvent(int width, int height) {
  _size = {width, height};
  _signalResize.emit(width, height);
}

Recti RenderWindow::regionAtFrameBuffer(const Recti& rect) const {
  int fbWidth, fbHeight;
  getDrawableSize(fbWidth, fbHeight);
  auto pixelRatio = (float)fbWidth / (float)_size.x;
  return {(int)(rect.x * pixelRatio), fbHeight - (int)((rect.y + rect.h) * pixelRatio), (int)(rect.w * pixelRatio),
          (int)(rect.h * pixelRatio)};
}

} // namespace fui
