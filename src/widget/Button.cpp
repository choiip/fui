#include "widget/Button.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

#define ICON_LOGIN 0xE740

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

void Button::draw(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  const auto& buttonStyle = style();
  auto x = _position.x;
  auto y = _position.y;
  auto w = _size.x;
  auto h = _size.y;
  NVGcolor col = {_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a};
  bool isblack = isBlack(_backgroundColor);
  const char* text = _caption.c_str();
  int preicon = _icon;

  NVGpaint bg;
  char icon[8];
  float cornerRadius = buttonStyle.buttonCornerRadius;
  float tw = 0, iw = 0;

  bg = nvgLinearGradient(vg, x, y, x, y + h, nvgRGBA(255, 255, 255, isblack ? 16 : 32),
                         nvgRGBA(0, 0, 0, isblack ? 16 : 32));
  nvgBeginPath(vg);
  nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, cornerRadius - 1);
  if (!isblack) {
    nvgFillColor(vg, col);
    nvgFill(vg);
  }
  nvgFillPaint(vg, bg);
  nvgFill(vg);

  nvgBeginPath(vg);
  nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, cornerRadius - 0.5f);
  nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
  nvgStroke(vg);

  nvgFontSize(vg, 20.0f);
  nvgFontFaceId(vg, buttonStyle.fontBold);
  tw = nvgTextBounds(vg, 0, 0, text, NULL, NULL);
  if (preicon != 0) {
    nvgFontSize(vg, h * 1.3f);
    nvgFontFaceId(vg, buttonStyle.fontIcons);
    iw = nvgTextBounds(vg, 0, 0, cpToUTF8(preicon, icon), NULL, NULL);
    iw += h * 0.15f;
  }

  if (preicon != 0) {
    nvgFontSize(vg, h * 1.3f);
    nvgFontFaceId(vg, buttonStyle.fontIcons);
    nvgFillColor(vg, nvgRGBA(255, 255, 255, 96));
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText(vg, x + w * 0.5f - tw * 0.5f - iw * 0.75f, y + h * 0.5f, cpToUTF8(preicon, icon), NULL);
  }

  nvgFontSize(vg, 20.0f);
  nvgFontFaceId(vg, buttonStyle.fontBold);
  nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
  nvgFillColor(vg, nvgRGBA(0, 0, 0, 160));
  nvgText(vg, x + w * 0.5f - tw * 0.5f + iw * 0.25f, y + h * 0.5f - 1, text, NULL);
  nvgFillColor(vg, nvgRGBA(255, 255, 255, 160));
  nvgText(vg, x + w * 0.5f - tw * 0.5f + iw * 0.25f, y + h * 0.5f, text, NULL);
}

} // namespace fui
