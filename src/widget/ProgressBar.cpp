#include "widget/ProgressBar.hpp"
#include <cmath>
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

#ifndef WIN32
using std::min;
using std::max;
#else
#include <windows.h>
#endif

namespace fui {
  
ProgressBar::ProgressBar(WidgetContainer* parent)
: Widget(parent) {}

void ProgressBar::draw(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  const auto& progressBarStyle = style();
  auto x = _position.x;
  auto y = _position.y;
  auto w = _size.x;
  auto h = _size.y;

  auto bgColorGradTop = nvgRGBA(0, 0, 0, 32);
  auto bgColorGradBot = nvgRGBA(0, 0, 0, 92);

  auto value = min(max(0, _value), _maxValue);
  auto barPos = (int)std::round((w - 2) * value / (float)_maxValue);

  auto barColorGradTop = nvgRGBA(220, 220, 220, 100);
  auto barColorGradBot = nvgRGBA(128, 128, 128, 100);

  auto textFontSize = progressBarStyle.buttonFontSize;
  auto tColor = reinterpret_cast<const NVGcolor*>(&progressBarStyle.standardTextColor);
  auto tShadow = reinterpret_cast<const NVGcolor*>(&progressBarStyle.textShadow);
  auto text = _text.c_str();

  NVGpaint paint = nvgBoxGradient(vg, x + 1, y + 1, w - 2, h, 3, 4, bgColorGradTop, bgColorGradBot);
  nvgBeginPath(vg);
  nvgRoundedRect(vg, x, y, w, h, 3);
  nvgFillPaint(vg, paint);
  nvgFill(vg);

  paint = nvgBoxGradient(vg, x, y, barPos + 1.5f, h - 1, 3, 4, barColorGradTop, barColorGradBot);
  nvgBeginPath(vg);
  nvgRoundedRect(vg, x + 1, y + 1, barPos, h - 2, 3);
  nvgFillPaint(vg, paint);
  nvgFill(vg);

  if (!_text.empty()) {
    nvgFontSize(vg, textFontSize);
    nvgFontFaceId(vg, progressBarStyle.fontBold);
    auto tw = nvgTextBounds(vg, 0, 0, text, NULL, NULL);
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, *tShadow);
    nvgText(vg, x + w * 0.5f - tw * 0.5f, y + h * 0.5f - 1, text, NULL);
    nvgFillColor(vg, *tColor);
    nvgText(vg, x + w * 0.5f - tw * 0.5f, y + h * 0.5f, text, NULL);  
  }
}

} // namespace fui
