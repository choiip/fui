
#pragma once

#include "Widget.hpp"

namespace fui {

/**
 * \class Canvas widget/Canvas.hpp
 *
 * \brief Widget to render graphics scene.
 */
class Canvas : public Widget {
public:
  explicit Canvas(WidgetContainer* parent);

  virtual ~Canvas() = default;

  /// Draw the canvas
  virtual void draw(RenderContext& renderContext) override final;

protected:
  virtual int drawScene(RenderContext& renderContext) = 0;

  //   virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
  //   virtual void onMousePressEvent(MouseEvent& event) override;
  //   virtual void onMouseReleaseEvent(MouseEvent& event) override;

  /// The canvas border width
  FUI_WIDGET_PROPERTY(float, borderWidth, 1.0f);
};

} // namespace fui
