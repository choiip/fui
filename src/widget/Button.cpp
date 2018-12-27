#include "widget/Button.hpp"
#include "core/Log.hpp"
#include "core/RenderContext.hpp"
#include "event/MouseEvent.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

namespace fui {

static char* cpToUTF8(int cp, char* str) {
  int n = 0;
  if (cp < 0x80)
    n = 1;
  else if (cp < 0x800)
    n = 2;
  else if (cp < 0x10000)
    n = 3;
  else if (cp < 0x200000)
    n = 4;
  else if (cp < 0x4000000)
    n = 5;
  else if (cp <= 0x7fffffff)
    n = 6;
  str[n] = '\0';
  switch (n) {
  case 6:
    str[5] = 0x80 | (cp & 0x3f);
    cp = cp >> 6;
    cp |= 0x4000000;
  case 5:
    str[4] = 0x80 | (cp & 0x3f);
    cp = cp >> 6;
    cp |= 0x200000;
  case 4:
    str[3] = 0x80 | (cp & 0x3f);
    cp = cp >> 6;
    cp |= 0x10000;
  case 3:
    str[2] = 0x80 | (cp & 0x3f);
    cp = cp >> 6;
    cp |= 0x800;
  case 2:
    str[1] = 0x80 | (cp & 0x3f);
    cp = cp >> 6;
    cp |= 0xc0;
  case 1:
    str[0] = cp;
  }
  return str;
}

Button::Button(WidgetContainer* parent, const Text& caption)
: Widget(parent)
, _caption(caption) {}

Button::~Button() {}

void Button::onMouseMoveEvent(MouseMoveEvent& event) {
  if (event.movement == Movement::ENTERING) {
    if (((event.buttons & MouseButton::LEFT) != MouseButton::NONE) && not pushed()) {
      pushed(true);
    }
  }
  if (event.movement == Movement::LEAVING) {
    if (pushed()) {
      pushed(false);
    }
  }
}

void Button::onMousePressEvent(MouseEvent& event) {
  if (event.button == MouseButton::LEFT) {
    pushed(true);
  }
}

void Button::onMouseReleaseEvent(MouseEvent& event) {
  if (event.button == MouseButton::LEFT) {
    auto lastPushState = pushed();
    if (lastPushState) {
      pushed(false);
      _signalPressed.emit();
    }
    if (lastPushState == !pushed()) {
      _signalToggled.emit(pushed());
    }
  }
}

void Button::draw(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  const auto& buttonStyle = style();
  auto x = _position.x;
  auto y = _position.y;
  auto w = _size.x;
  auto h = _size.y;
  auto bgColor = reinterpret_cast<const NVGcolor*>(&_backgroundColor);
  auto borderColor = nvgRGBA(0, 0, 0, 48);
  auto tColor = reinterpret_cast<const NVGcolor*>(
      _enabled ? (_textColor.a > 0 ? &_textColor : &buttonStyle.standardTextColor) : &buttonStyle.disabledTextColor);
  auto tShadow = reinterpret_cast<const NVGcolor*>(&buttonStyle.textShadow);

  const char* text = _caption.c_str();
  auto gradTop = *reinterpret_cast<const NVGcolor*>(_pushed ? &buttonStyle.buttonGradientTopPushed
                                                            : &buttonStyle.buttonGradientTopNormal);
  auto gradBot = *reinterpret_cast<const NVGcolor*>(_pushed ? &buttonStyle.buttonGradientBotPushed
                                                            : &buttonStyle.buttonGradientBotNormal);

  auto textFontSize = buttonStyle.buttonFontSize;
  auto iconFontSize = h * 1.3f;
  int preicon = _icon;

  NVGpaint bg;
  char icon[8];
  float cornerRadius = buttonStyle.buttonCornerRadius;
  float tw = 0, iw = 0;

  bg = nvgLinearGradient(vg, x, y, x, y + h, gradTop, gradBot);
  nvgBeginPath(vg);
  nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, cornerRadius - 1);
  if (bgColor->a > .0f) {
    nvgFillColor(vg, *bgColor);
    nvgFill(vg);
  }
  nvgFillPaint(vg, bg);
  nvgFill(vg);

  nvgBeginPath(vg);
  nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, cornerRadius - 0.5f);
  nvgStrokeColor(vg, borderColor);
  nvgStroke(vg);

  nvgFontSize(vg, textFontSize);
  nvgFontFaceId(vg, buttonStyle.fontBold);
  tw = nvgTextBounds(vg, 0, 0, text, NULL, NULL);
  if (preicon != 0) {
    nvgFontSize(vg, iconFontSize);
    nvgFontFaceId(vg, buttonStyle.fontIcons);
    iw = nvgTextBounds(vg, 0, 0, cpToUTF8(preicon, icon), NULL, NULL);
    iw += h * 0.15f;

    nvgFillColor(vg, *tColor);
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText(vg, x + w * 0.5f - tw * 0.5f - iw * 0.75f, y + h * 0.5f, icon, NULL);
  }

  nvgFontSize(vg, textFontSize);
  nvgFontFaceId(vg, buttonStyle.fontBold);
  nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
  nvgFillColor(vg, *tShadow);
  nvgText(vg, x + w * 0.5f - tw * 0.5f + iw * 0.25f, y + h * 0.5f - 1, text, NULL);
  nvgFillColor(vg, *tColor);
  nvgText(vg, x + w * 0.5f - tw * 0.5f + iw * 0.25f, y + h * 0.5f, text, NULL);
}

} // namespace fui
