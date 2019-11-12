#include "core/RenderContext.hpp"
#include <memory>
#define STB_DEFINE
#include <stb.h>
#include <stb_image.h>
#include "image/mjpeg2jpeg.h"
#include "nanovg/nanovg.h"

namespace fui {

RenderContext::RenderContext()
: _vg(nullptr) {}

RenderContext::~RenderContext() = default;

int RenderContext::loadFont(const std::string& name, const std::string& filename) {
  return nvgCreateFont(_vg, name.c_str(), filename.c_str());
}

int RenderContext::loadFont(const std::string& name, const void* fontData, int length) {
  return nvgCreateFontMem(_vg, name.c_str(), (unsigned char*)fontData, length, 0);
}

int RenderContext::loadImage(const std::string& filename) { return nvgCreateImage(vg(), filename.c_str(), 0); }

int RenderContext::createImage(int width, int height, PixelFormat format) {
  constexpr NVGtexture nvgFormat[] = {NVG_TEXTURE_RGB, NVG_TEXTURE_RGBA, NVG_TEXTURE_YUYV};
  return nvgCreateImageWithType(_vg, nvgFormat[(int)format], width, height, 0, nullptr);
}

void RenderContext::updateImage(int image, const unsigned char* data, size_t dataSize, bool compressed) {
  if (compressed) { // TODO: Better support for other formats
    int width = 0, height = 0;
    nvgImageSize(_vg, image, &width, &height);

    // handle AVI1 (MJPEG)
    uint8_t* jpegBuf;
    int jpegLen;
    int status = mjpeg2jpeg(&jpegBuf, &jpegLen, data, dataSize);
    if (status) {
      int w, h, n;
      auto pixels = stbi_load_from_memory(jpegBuf, jpegLen, &w, &h, &n, 3);
      if (pixels == NULL) {
        nvgErrorPrint("Failed to load from data - %s\n", stbi_failure_reason());
        free(jpegBuf);
        return;
      }
      if (width == w && height == h) { nvgUpdateImage(_vg, image, pixels); }
      stbi_image_free(pixels);
    }
    free(jpegBuf);
  } else {
    nvgUpdateImage(_vg, image, data);
  }
}

} // namespace fui
