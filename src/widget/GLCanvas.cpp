#include "widget/GLCanvas.hpp"
#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"

namespace fui {

GLCanvas::GLCanvas(WidgetContainer* parent)
: Canvas(parent)
{}

int GLCanvas::drawScene(RenderContext& renderContext) {
  auto vg = renderContext.vg();
  auto w = _size.x, h = _size.y;
  if (!_renderTarget.empty()) {
    nvgImageSize(vg, _renderTarget[0], &w, &h);
  }
  nvgBindFramebuffer(vg, _framebuffer);
  _drawFunction(w, h);
  nvgBindFramebuffer(vg, 0);

  return _showTarget < _renderTarget.size() ? _renderTarget[_showTarget] : 0;
}

auto GLCanvas::_renderTargetSetter(const std::vector<int>& renderTarget, NVGcontext* ctx) -> decltype(this) {
  for (auto rt: _renderTarget) {
    nvgDeleteImage(ctx, rt);
  }
  _renderTarget = renderTarget;
  if (!renderTarget.empty()) {
    auto attachments = 0;
    for (size_t i=0; i<renderTarget.size(); ++i) {
      attachments |= (NVG_FBA_COLOR0 << i);
    }
    nvgAttachFramebuffer(ctx, _framebuffer, _renderTarget.data(), attachments);
  }
  return this;
}

auto GLCanvas::_framebufferSetter(int width, int height, int imageFlags, NVGcontext* ctx) -> decltype(this) {
  if (_framebuffer != 0) nvgDeleteFramebuffer(ctx, _framebuffer);
  _framebuffer = nvgCreateFramebuffer(ctx);
  auto colorImage = nvgCreateImageRGBA(ctx, width, height, imageFlags | NVG_IMAGE_FLIPY | NVG_IMAGE_PREMULTIPLIED, NULL);
  nvgAllocateFramebufferAttachment(ctx, _framebuffer, width, height, NVG_FBA_DEPTH | NVG_FBA_STENCIL);
  std::vector<int> newRenderTarget = { colorImage };
  return renderTarget(newRenderTarget, ctx);
}

} // namespace fui
