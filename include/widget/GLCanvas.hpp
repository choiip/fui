#pragma once

#include "Canvas.hpp"

namespace fui {

/**
 * \class GLCanvas widget/GLCanvas.hpp
 *
 * \brief Widget to render graphics scene in OpenGL.
 */
class GLCanvas : public Canvas {
public:
  GLCanvas(WidgetContainer* parent);

  virtual ~GLCanvas() = default;

protected:
  virtual void drawScene() override;

  FUI_WIDGET_WRITEONLY_PROPERTY(std::function<void()>, drawFunction, nullptr);
};

} // namespace fui
