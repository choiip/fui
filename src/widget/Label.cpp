#include "widget/Label.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

namespace fui {

Label::Label(WidgetContainer* parent, const Text& caption)
: Widget(parent)
, _caption(caption) {}

void Label::draw(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  const auto& labelStyle = style();
  auto tColor = reinterpret_cast<const NVGcolor*>(&_textColor);
  auto textFontSize = labelStyle.standardFontSize;

  nvgFontFaceId(vg, labelStyle.fontStandard);
  nvgFontSize(vg, textFontSize);
  nvgFillColor(vg, *tColor);
  if (_fixedSize.x > 0) {
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgTextBox(vg, _position.x, _position.y, _fixedSize.x, _caption.c_str(), nullptr);
  } else {
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText(vg, _position.x, _position.y + _size.y * 0.5f, _caption.c_str(), nullptr);
  }
}

} // namespace fui
