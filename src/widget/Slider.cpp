#include "widget/Slider.hpp"
#include "core/RenderContext.hpp"
#include "core/Tooltip.hpp"
#include "event/MouseEvent.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

namespace fui {

struct Slider::SnapState {
  float startX;
  float widthX;
};

Slider::Slider(WidgetContainer* parent)
: Widget(parent) {}

void Slider::draw(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  const auto& sliderStyle = style();
  auto borderLight = reinterpret_cast<const NVGcolor*>(&sliderStyle.borderLight);
  auto borderMedium = reinterpret_cast<const NVGcolor*>(&sliderStyle.borderMedium);
  auto borderDark = reinterpret_cast<const NVGcolor*>(&sliderStyle.borderDark);

  auto hlColor = reinterpret_cast<const NVGcolor*>(&_highlightColor);

  auto center = _position.y + _size.y * 0.5f;
  float kr = (int)(_size.y * 0.4f), kshadow = 3;

  float startX = kr + kshadow + _position.x;
  float widthX = _size.x - 2 * (kr + kshadow);

  Vector2f knobPos = {  startX + (_value - _range.first) / (_range.second - _range.first) * widthX,
                        center + 0.5f };

  NVGpaint bg = nvgBoxGradient(vg, startX, center - 3 + 1, widthX, 6, 3,
                                3, NVGcolor{ 0, 0, 0, enabled() ? 32/255.f : 10/255.f },
                                NVGcolor{ 0, enabled() ? 128/255.f : 210/255.f});

  nvgBeginPath(vg);
  nvgRoundedRect(vg, startX, center - 3 + 1, widthX, 6, 2);
  nvgFillPaint(vg, bg);
  nvgFill(vg);

  if (_highlightedRange.second != _highlightedRange.first) {
      nvgBeginPath(vg);
      nvgRoundedRect(
          vg, startX + _highlightedRange.first * _size.x,
          center - kshadow + 1,
          widthX * (_highlightedRange.second - _highlightedRange.first),
          kshadow * 2, 2);
      nvgFillColor(vg, *hlColor);
      nvgFill(vg);
  }

  NVGpaint knobShadow =
      nvgRadialGradient(vg, knobPos.x, knobPos.y, kr - kshadow,
                        kr + kshadow, NVGcolor{0, 0, 0, 64/255.f}, NVGcolor{0, 0, 0, 0});

  nvgBeginPath(vg);
  nvgRect(vg, knobPos.x - kr - 5, knobPos.y - kr - 5, kr * 2 + 10,
          kr * 2 + 10 + kshadow);
  nvgCircle(vg, knobPos.x, knobPos.y, kr);
  nvgPathWinding(vg, NVG_HOLE);
  nvgFillPaint(vg, knobShadow);
  nvgFill(vg);

  NVGpaint knob = nvgLinearGradient(
      vg, _position.x, center - kr, _position.x, center + kr,
      *borderLight, *borderMedium);
  NVGpaint knobReverse = nvgLinearGradient(
      vg, _position.x, center - kr, _position.x, center + kr,
      *borderMedium, *borderLight);

  nvgBeginPath(vg);
  nvgCircle(vg, knobPos.x, knobPos.y, kr);
  nvgStrokeColor(vg, *borderDark);
  nvgFillPaint(vg, knob);
  nvgStroke(vg);
  nvgFill(vg);
  nvgBeginPath(vg);
  nvgCircle(vg, knobPos.x, knobPos.y, kr * 0.5f);
  nvgFillColor(vg, NVGcolor{150/255.f, 150/255.f, 150/255.f, enabled() ? 1.f : 100/255.f});
  nvgStrokePaint(vg, knobReverse);
  nvgStroke(vg);
  nvgFill(vg);
}

void Slider::prepareTooltip(Tooltip& tooltip) {
  float kr = (int)(_size.y * 0.4f), kshadow = 3;

  float startX = kr + kshadow;
  float widthX = _size.x - 2 * (kr + kshadow);

  float knobPositionX = startX + (_value - _range.first) / (_range.second - _range.first) * widthX;

  Recti rect = { (int)knobPositionX, _size.y, 0, 0 };
  auto desiredPosition = mapTo(rect, Coordinate::TopParent).position;
  tooltip.string(std::to_string(_value))
        ->absolutePosition(desiredPosition)
        ->bubble(Tooltip::Bubble::TOP);
}

void Slider::onMouseMoveEvent(MouseMoveEvent& event) {
  if (_snap) {
    float value = (event.position.x - _snap->startX) / _snap->widthX;
    value = value * (_range.second - _range.first) + _range.first;
    value = std::min(std::max(value, _range.first), _range.second);
    if (_value != value) {
      _value = value;
      _signalValueChanged.emit(value);
    }
  }
}

void Slider::onMousePressEvent(MouseEvent& event) {
  const float kr = (int)(_size.y * 0.4f), kshadow = 3;
  const float startX = kr + kshadow + _position.x - 1;
  const float widthX = _size.x - 2 * (kr + kshadow);

  _snap.reset(new SnapState { startX, widthX } );

  auto value = (event.position.x - _snap->startX) / _snap->widthX;
  value = value * (_range.second - _range.first) + _range.first;
  value = std::min(std::max(value, _range.first), _range.second);
  if (_value != value) {
    _value = value;
    _signalValueChanged.emit(value);
  }
}

void Slider::onMouseReleaseEvent(MouseEvent& event) {
  if (_snap) {
    _snap.reset();
  }
}

} // namespace fui
