#pragma once

#include "Widget.hpp"

namespace fui {

/**
 * \class ProgressBar widget/ProgressBar.hpp
 *
 * \brief Standard widget for visualizing progress.
 */
class ProgressBar : public Widget {
public:
  explicit ProgressBar(WidgetContainer* parent);
  virtual ~ProgressBar() = default;

  /// Draw the progress bar
  virtual void draw(RenderContext& renderContext) override;

protected:
  /// The maximun value
  FUI_WIDGET_PROPERTY(int, maxValue, 100);

  /// The descriptive text shown with the progress bar
  FUI_WIDGET_PROPERTY(Text, text, "");

  /// The progress value
  FUI_WIDGET_PROPERTY(int, value, 0);
};

} // namespace fui
