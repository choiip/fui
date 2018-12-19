#include "core/RenderWindow.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"

namespace fui {

RenderWindow::RenderWindow()
: WidgetContainer(nullptr) {}

void RenderWindow::drawGui() {
  auto ctx = renderContext();
  if (ctx && _visible) {
    int winWidth, winHeight, fbWidth, fbHeight;
    getWindowSize(winWidth, winHeight);
    getDrawableSize(fbWidth, fbHeight);
    // Calculate pixel ration for hi-dpi devices.
    auto pxRatio = (float)fbWidth / (float)winWidth;

    auto vg = ctx->vg();
    nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
    WidgetContainer::draw(*ctx);
    nvgEndFrame(vg);
  }
}

} // namespace fui
