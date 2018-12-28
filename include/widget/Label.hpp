#pragma once

#include "Widget.hpp"

namespace fui {

/**
 * \class Label widget/Label.hpp
 *
 * \brief Text label widget.
 */
class Label : public Widget {
public:
  Label(WidgetContainer* parent, const Text& caption);
  virtual ~Label() = default;

  /// Draw the label
  virtual void draw(RenderContext& renderContext) override;

protected:
  /// The label's text caption
  FUI_WIDGET_PROPERTY(Text, caption, "");
  /// The label color
  FUI_WIDGET_PROPERTY(Color, textColor, {0, 0, 0, 1});
};

} // namespace fui
