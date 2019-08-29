#pragma once

#include <vector>
#include "Canvas.hpp"

struct NVGcontext;

namespace fui {

/**
 * \class GLCanvas widget/GLCanvas.hpp
 *
 * \brief Widget to render graphics scene in OpenGL.
 */
class GLCanvas : public Canvas {
public:
  explicit GLCanvas(WidgetContainer* parent);

  virtual ~GLCanvas() = default;

protected:
  virtual int drawScene(RenderContext& renderContext) override;

  FUI_WIDGET_PROPERTY_ALT_SETTER(std::vector<int>, renderTarget, {});

  FUI_WIDGET_PROPERTY(int, showTarget, 0);

  FUI_WIDGET_WRITEONLY_PROPERTY(std::function<void(int, int)>, drawFunction, nullptr);

  FUI_WIDGET_WRITEONLY_PROPERTY_ALT(int, framebuffer, 0);

private:
  auto _renderTargetSetter(const std::vector<int>& renderTarget, NVGcontext* ctx) -> decltype(this);

  // setup the framebuffer with desired size
  auto _framebufferSetter(int width, int height, int imageFlags, NVGcontext* ctx) -> decltype(this);
};

} // namespace fui
