#include "core/RenderWindow.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"
#include "widget/WidgetStyle.hpp"

namespace fui {

RenderWindow::RenderWindow()
: WidgetContainer(nullptr) {}

void RenderWindow::drawGui() {
  auto ctx = renderContext();

  if (ctx && _visible) {
    auto vg = ctx->vg();
    // load default sytle
    static auto defaultStyle = std::make_shared<WidgetStyle>(vg);
    Widget::setStyle(defaultStyle);

    int winWidth, winHeight, fbWidth, fbHeight;
    getWindowSize(winWidth, winHeight);
    getDrawableSize(fbWidth, fbHeight);
    // Calculate pixel ration for hi-dpi devices.
    auto pxRatio = (float)fbWidth / (float)winWidth;

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
    WidgetContainer::draw(*ctx);
    nvgEndFrame(vg);
  }
}

} // namespace fui
