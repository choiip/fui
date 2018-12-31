
#pragma once

#include "WidgetContainer.hpp"

namespace fui {

/**
 * \class Window widget/Window.hpp
 *
 * \brief Top-level window widget.
 */
class Window : public WidgetContainer {
public:
  Window(WidgetContainer* parent, const Text& title = "Untitled");

  virtual ~Window();

  /// Draw the window
  virtual void draw(RenderContext& renderContext) override;

protected:
  virtual void onMouseMoveEvent(MouseMoveEvent& event) override;
  virtual void onMousePressEvent(MouseEvent& event) override;
  virtual void onMouseReleaseEvent(MouseEvent& event) override;

  /// The window title
  FUI_WIDGET_PROPERTY(Text, title, "");

private:
  struct SnapState;
  std::unique_ptr<SnapState> _snap;
};

} // namespace fui
