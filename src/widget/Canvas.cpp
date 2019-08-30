#include "widget/Canvas.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

namespace fui {

Canvas::Canvas(WidgetContainer* parent)
: Widget(parent)
{}

void Canvas::draw(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  const auto& canvasStyle = style();
  auto x = _position.x;
  auto y = _position.y;
  auto w = _size.x;
  auto h = _size.y;

  auto borderColor = reinterpret_cast<const NVGcolor*>(&canvasStyle.windowPopup);
  auto borderShadow = reinterpret_cast<const NVGcolor*>(&canvasStyle.borderDark);

  auto renderTarget = drawScene(renderContext);

  // draw image
  auto imgPaint = nvgImagePattern(vg, x, y, w, h, 0.f, renderTarget, 1.0f);
  nvgBeginPath(vg);
  nvgRoundedRect(vg, x, y, w, h, 5);
  nvgFillPaint(vg, imgPaint);
  nvgFill(vg);

  if (_borderWidth > 0) {
    nvgBeginPath(vg);
    nvgStrokeWidth(vg, _borderWidth);
    nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, 0);
    nvgStrokeColor(vg, *borderColor);
    nvgStroke(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, canvasStyle.buttonCornerRadius);
    nvgStrokeColor(vg, *borderShadow);
    nvgStroke(vg);
  }
}

} // namespace fui
