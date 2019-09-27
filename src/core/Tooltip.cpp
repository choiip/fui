#include "core/Tooltip.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"

namespace fui {

void Tooltip::draw(RenderContext& renderContext) {
  if (_string.empty()) return;

  auto vg = renderContext.vg();

  auto pos = _absolutePosition;
  auto tooltipString = _string;
  auto tooltipWidth = _width;
  auto fontSize = _fontSize;
  auto fontId = _font == -1 ? nvgFindFont(vg, "sans") : _font;
  float bounds[4];

  nvgFontFaceId(vg, fontId);
  nvgFontSize(vg, fontSize);
  nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
  nvgTextLineHeight(vg, 1.1f);

  nvgTextBounds(vg, pos.x, pos.y, tooltipString.c_str(), nullptr, bounds);
  int halfw = (bounds[2] - bounds[0]) * 0.5f;
  if (halfw > tooltipWidth / 2) {
    nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
    nvgTextBoxBounds(vg, pos.x, pos.y, tooltipWidth, tooltipString.c_str(), nullptr, bounds);

    halfw = (bounds[2] - bounds[0]) * 0.5f;
  }

  nvgBeginPath(vg);
  nvgFillColor(vg, NVGcolor{0, 0, 0, 1});
  nvgRoundedRect(vg, bounds[0] - 4 - halfw, bounds[1] - 4, (int)(bounds[2] - bounds[0]) + 8,
                 (int)(bounds[3] - bounds[1]) + 8, 3);

  int px = (int)((bounds[2] + bounds[0]) * 0.5f) - halfw;
  if (_bubble == Bubble::TOP) {
    nvgMoveTo(vg, px, bounds[3] + 10);
    nvgLineTo(vg, px + 7, bounds[3] - 1);
    nvgLineTo(vg, px - 7, bounds[3] - 1);
  } else {
    nvgMoveTo(vg, px, bounds[1] - 10);
    nvgLineTo(vg, px + 7, bounds[1] + 1);
    nvgLineTo(vg, px - 7, bounds[1] + 1);
  }
  nvgFill(vg);

  nvgFillColor(vg, NVGcolor{1, 1, 1, 1});
  nvgFontBlur(vg, 0.0f);
  nvgTextBox(vg, pos.x - halfw, pos.y, tooltipWidth, tooltipString.c_str(), nullptr);
}

} // namespace fui
