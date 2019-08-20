#include "widget/Window.hpp"
#include "core/Log.hpp"
#include "core/RenderContext.hpp"
#include "event/MouseEvent.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

namespace fui {

struct Window::SnapState {
  Vector2i relativePosition;
};

Window::Window(WidgetContainer* parent, const Text& title)
: WidgetContainer(parent)
, _title(title) {}

Window::~Window() {}

void Window::draw(RenderContext& renderContext) {
  static const auto transparent = nvgRGBA(0, 0, 0, 0);
  static const auto dropShadow = nvgRGBA(0, 0, 0, 128);

  auto vg = renderContext.vg();
  const auto& windowStyle = style();
  auto x = _position.x;
  auto y = _position.y;
  auto w = _size.x;
  auto h = _size.y;

  bool mouseHover = false;
  auto bgColor =
      reinterpret_cast<const NVGcolor*>(mouseHover ? &windowStyle.windowFillFocused : &windowStyle.windowFillNormal);
  auto text = _title.c_str();
  auto textColor =
      reinterpret_cast<const NVGcolor*>(mouseHover ? &windowStyle.windowTitleFocused : &windowStyle.windowTitleNormal);
  auto gradTop = *reinterpret_cast<const NVGcolor*>(&windowStyle.windowHeaderGradientTop);
  auto gradBot = *reinterpret_cast<const NVGcolor*>(&windowStyle.windowHeaderGradientBot);
  int ds = windowStyle.windowDropShadowSize, cr = windowStyle.windowCornerRadius;
  int hh = windowStyle.windowHeaderHeight;

  /* Draw window */
  nvgBeginPath(vg);
  nvgRoundedRect(vg, x, y, w, h, cr);

  nvgFillColor(vg, *bgColor);
  nvgFill(vg);

  /* Draw a drop shadow */
  NVGpaint shadowPaint = nvgBoxGradient(vg, x, y, w, h, cr * 2, ds * 2, dropShadow, transparent);

  nvgSave(vg);
  nvgResetScissor(vg);
  nvgBeginPath(vg);
  nvgRect(vg, x - ds, y - ds, w + 2 * ds, h + 2 * ds);
  nvgRoundedRect(vg, x, y, w, h, cr);
  nvgPathWinding(vg, NVG_HOLE);
  nvgFillPaint(vg, shadowPaint);
  nvgFill(vg);
  nvgRestore(vg);

  if (!_title.empty()) {
    auto headerTopSep = reinterpret_cast<const NVGcolor*>(&windowStyle.windowHeaderTopSep);
    auto headerBotSep = reinterpret_cast<const NVGcolor*>(&windowStyle.windowHeaderBotSep);

    /* Draw header */
    NVGpaint headerPaint = nvgLinearGradient(vg, x, y, x, y + hh, gradTop, gradBot);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, w, hh, cr);

    nvgFillPaint(vg, headerPaint);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, w, hh, cr);
    nvgStrokeColor(vg, *headerTopSep);

    nvgSave(vg);
    nvgIntersectScissor(vg, x, y, w, 0.5f);
    nvgStroke(vg);
    nvgRestore(vg);

    nvgBeginPath(vg);
    nvgMoveTo(vg, x + 0.5f, y + hh - 1.5f);
    nvgLineTo(vg, x + w - 0.5f, y + hh - 1.5);
    nvgStrokeColor(vg, *headerBotSep);
    nvgStroke(vg);

    nvgFontSize(vg, 18.0f);
    nvgFontFaceId(vg, windowStyle.fontBold);
    nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    nvgFontBlur(vg, 2);
    nvgFillColor(vg, dropShadow);
    nvgText(vg, x + w / 2, y + hh / 2, text, nullptr);

    nvgFontBlur(vg, 0);
    nvgFillColor(vg, *textColor);
    nvgText(vg, x + w / 2, y + hh / 2 - 1, text, nullptr);
  }

  WidgetContainer::draw(renderContext);
}

Vector2i Window::childrenOrigin() const {
  auto origin = WidgetContainer::childrenOrigin();
  origin.y += style().windowHeaderHeight;
  return origin;
}

void Window::onMouseMoveEvent(MouseMoveEvent& event) {
  if (_snap && hasFlags(event.buttons, MouseButton::LEFT)) {
    _position.x = event.position.x - _snap->relativePosition.x;
    _position.y = event.position.y - _snap->relativePosition.y;
    // bound y to be non-negative
    if (_position.y < 0) {
      _position.y = 0;
    }
  } else {
    auto headerHeight = style().windowHeaderHeight;
    MouseMoveEvent altEvent({event.prevPosition.x, event.prevPosition.y - headerHeight}, event.movement, {event.position.x, event.position.y - headerHeight}, event.button, event.buttons,
                              event.modifiers);    
    WidgetContainer::onMouseMoveEvent(altEvent);
  }
}

void Window::onMousePressEvent(MouseEvent& event) {
  auto headerHeight = style().windowHeaderHeight;
  auto hitHeaderArea = false;
  if (event.button == MouseButton::LEFT) {
    hitHeaderArea = (event.position.y - _position.y < headerHeight);
  }
  if (hitHeaderArea) {
    _snap.reset(new SnapState);
    _snap->relativePosition.x = event.position.x - _position.x;
    _snap->relativePosition.y = event.position.y - _position.y;
  } else {
    MouseEvent altEvent = {{event.position.x, event.position.y - headerHeight}, event.button, event.buttons, event.modifiers};
    WidgetContainer::onMousePressEvent(altEvent);
  }
}

void Window::onMouseReleaseEvent(MouseEvent& event) {
  if (_snap && event.button == MouseButton::LEFT) {
    _snap.reset();
  }
  WidgetContainer::onMouseReleaseEvent(event);
}

} // namespace fui
