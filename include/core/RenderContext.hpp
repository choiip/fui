#pragma once

#include <string>

struct NVGcontext;

namespace fui {

struct Color;

enum class PixelFormat {
  RGB   = 0,
  RGBA  = 1,
  YUYV  = 2,
};

class RenderContext {
public:
  RenderContext();
  virtual ~RenderContext();
  NVGcontext* vg() const { return _vg; }

  int loadFont(const std::string& name, const std::string& filename);
  int loadFont(const std::string& name, const void* fontData, int length);

  int loadImage(const std::string& filename);
  int createImage(int width, int height, PixelFormat format);
  void updateImage(int image, const unsigned char* data, size_t dataSize, bool compressed = false);

  std::string loadVertexShader(const std::string& filename);
  std::string loadFragmentShader(const std::string& filename);
  
  virtual auto setViewport(int x, int y, int width, int height) -> decltype(this) = 0;
  virtual auto preDraw(const Color* clearColor = nullptr, const float* clearDepth = nullptr, const int* clearStencil = nullptr) -> decltype(this) = 0;
  virtual auto postDraw() -> decltype(this) = 0;

protected:
  virtual std::string versionLine() const = 0;
  virtual std::string vertexMacro() const = 0;
  virtual std::string fragmentMacro() const = 0;

  NVGcontext* _vg;
};

} // namespace fui
