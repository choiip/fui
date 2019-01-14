#pragma once

#include <string>

struct NVGcontext;

namespace fui {

enum class PixelFormat {
  RGB   = 0,
  RGBA  = 1,
  YUYV  = 2,
};

class RenderContext {
public:
  RenderContext();
  NVGcontext* vg() const { return _vg; }

  int loadFont(const std::string& name, const std::string& filename);
  int loadFont(const std::string& name, const void* fontData, int length);

  int loadImage(const std::string& filename);
  int createImage(int width, int height, PixelFormat format);
  void updateImage(int image, const unsigned char* data, size_t dataSize, bool compressed = false);

protected:
  NVGcontext* _vg;
};

} // namespace fui
