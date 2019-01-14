#include "core/RenderContext.hpp"
#include <memory>
#include "image/jpg_decompress.h"
#include "nanovg/nanovg.h"

namespace fui {

RenderContext::RenderContext()
: _vg(nullptr) {}

int RenderContext::loadFont(const std::string& name, const std::string& filename) {
  return nvgCreateFont(_vg, name.c_str(), filename.c_str());
}

int RenderContext::loadFont(const std::string& name, const void* fontData, int length) {
  return nvgCreateFontMem(_vg, name.c_str(), (unsigned char*)fontData, length, 0);
}

int RenderContext::loadImage(const std::string& filename) { return nvgCreateImage(vg(), filename.c_str(), 0); }

int RenderContext::createImage(int width, int height, PixelFormat format) {
  constexpr NVGtexture nvgFormat[] = { NVG_TEXTURE_RGB, NVG_TEXTURE_RGBA, NVG_TEXTURE_YUYV };
  return nvgCreateImageWithType(_vg, nvgFormat[(int)format], width, height, 0, nullptr);
}

void RenderContext::updateImage(int image, const unsigned char* data, size_t dataSize, bool compressed) { 
  if (compressed) {
    int width = 0, height = 0;
    nvgImageSize(_vg, image, &width, &height);
    size_t pixelSize = width * height * 3;
    std::unique_ptr<unsigned char[]> pixels;
    pixels.reset(new unsigned char[pixelSize]);
    unsigned int dWidth, dHeight, dChannels;
    jpg_decompress(data, dataSize, pixels.get(), pixelSize, &dWidth, &dHeight, &dChannels);
    if (width == dWidth && height == dHeight) {
      nvgUpdateImage(_vg, image, pixels.get());
    }
  } else {
    nvgUpdateImage(_vg, image, data);
  }
}

} // namespace fui
