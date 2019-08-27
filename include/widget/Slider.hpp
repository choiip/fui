#pragma once

#include "Widget.hpp"

namespace fui {

/**
 * \class Slider widget/Slider.hpp
 *
 * \brief Fractional slider widget.
 */
class Slider : public Widget {
public:
  Slider(WidgetContainer* parent);
  virtual ~Slider() = default;

  /// Draw the label
  virtual void draw(RenderContext& renderContext) override;

protected:
  /// Fill the incoming tooltip properties to displace the value of slider above/below the knob
  virtual void prepareTooltip(Tooltip& tooltip) override;

  virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
  virtual void onMousePressEvent(MouseEvent& event) override;
  virtual void onMouseReleaseEvent(MouseEvent& event) override;

  /// The pressed signal for all types of buttons.
  FUI_WIDGET_EVENT(void(float), ValueChanged);

  /// The slider value
  FUI_WIDGET_PROPERTY(float, value, 0);
  /// The range(min, max) of slider value
  FUI_WIDGET_PROPERTY(Range, range, {0, 1});

  FUI_WIDGET_PROPERTY(Range, highlightedRange, {0, 0});
  /// The highlight color
  FUI_WIDGET_PROPERTY(Color, highlightColor, {255, 80, 80, 70});

private:
  struct SnapState;
  std::unique_ptr<SnapState> _snap;  
};

} // namespace fui
