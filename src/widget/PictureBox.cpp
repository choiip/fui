#include "widget/PictureBox.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"
#include "core/Log.hpp"

namespace fui {

PictureBox::PictureBox(WidgetContainer* parent)
: Widget(parent) {}

PictureBox::~PictureBox() {}

auto PictureBox::center() -> decltype(this) {
  auto resultOffsetX = (int)((_size.x - _scale * _pictureSize.x) * .5f);
  auto resultOffsetY = (int)((_size.y - _scale * _pictureSize.y) * .5f);
  _offset = {resultOffsetX, resultOffsetY};
  return this;
}

auto PictureBox::fit() -> decltype(this) {
  auto resultScaleX = _size.x / static_cast<float>(_pictureSize.x);
  auto resultScaleY = _size.y / static_cast<float>(_pictureSize.y);
  return scale(resultScaleX < resultScaleY ? resultScaleX : resultScaleY)->center();
}

void PictureBox::draw(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  const auto& pictureBoxStyle = style();
  auto x = _position.x;
  auto y = _position.y;
  auto w = _size.x;
  auto h = _size.y;

  auto borderColor = reinterpret_cast<const NVGcolor*>(&pictureBoxStyle.windowPopup);
  auto borderShadow = reinterpret_cast<const NVGcolor*>(&pictureBoxStyle.borderDark);

  auto positionAfterOffset = _position + _offset;
  auto scaledPictureSizeX = _scale * _pictureSize.x;
  auto scaledPictureSizeY = _scale * _pictureSize.y;
  auto rotatePivotX = positionAfterOffset.x + scaledPictureSizeX * .5f;
  auto rotatePivotY = positionAfterOffset.y + scaledPictureSizeY * .5f;
  auto angle = _orientation / 180.0f * NVG_PI;

  // draw image border
  nvgBeginPath(vg);
  nvgStrokeWidth(vg, 1);
  nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, 0);
  nvgStrokeColor(vg, *borderColor);
  nvgStroke(vg);

  nvgBeginPath(vg);
  nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, pictureBoxStyle.buttonCornerRadius);
  nvgStrokeColor(vg, *borderShadow);
  nvgStroke(vg);

  // draw image
  auto imgPaint = nvgImagePattern(vg, positionAfterOffset.x, positionAfterOffset.y, scaledPictureSizeX,
                                  scaledPictureSizeY, 0.f, _picture, 1.0f);
  nvgSave(vg);
  nvgTranslate(vg, rotatePivotX, rotatePivotY);
  nvgRotate(vg, angle);
  nvgTranslate(vg, -rotatePivotX, -rotatePivotY);
  nvgBeginPath(vg);
  nvgRoundedRect(vg, positionAfterOffset.x, positionAfterOffset.y, scaledPictureSizeX, scaledPictureSizeY, 5);
  nvgFillPaint(vg, imgPaint);
  nvgFill(vg);
  nvgRestore(vg);

  // draw widget border
  nvgBeginPath(vg);
  nvgStrokeWidth(vg, 1);
  nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, 0);
  nvgStrokeColor(vg, *borderColor);
  nvgStroke(vg);

  nvgBeginPath(vg);
  nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, pictureBoxStyle.buttonCornerRadius);
  nvgStrokeColor(vg, *borderShadow);
  nvgStroke(vg);
}

auto PictureBox::_pictureSetter(int const& v, NVGcontext* ctx) -> decltype(this) {
  int w = 0, h = 0;
  _picture = v;
  nvgImageSize(ctx, v, &w, &h);
  _pictureSize = {w, h};
  return fit();
}

auto PictureBox::_pictureSetter(const std::string& file, NVGcontext* ctx) -> decltype(this) {
  auto picture = nvgCreateImage(ctx, file.c_str(), 0);
  if (picture == 0) {
    LOGE << "Could not load " << file;
  }
  return _pictureSetter(picture, ctx);
}
  
auto PictureBox::_scaleSetter(float const& v) -> decltype(this) {
  _scale = v > 0.01f ? v : 0.01f;
  return this;
}

} // namespace fui
