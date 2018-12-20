#include "widget/Button.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

namespace fui {

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
  int preicon = 0;

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
    // iw = nvgTextBounds(vg, 0,0, cpToUTF8(preicon,icon), NULL, NULL);
    iw += h * 0.15f;
  }

  if (preicon != 0) {
    nvgFontSize(vg, h * 1.3f);
    nvgFontFaceId(vg, buttonStyle.fontIcons);
    nvgFillColor(vg, nvgRGBA(255, 255, 255, 96));
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    // nvgText(vg, x+w*0.5f-tw*0.5f-iw*0.75f, y+h*0.5f, cpToUTF8(preicon,icon), NULL);
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
