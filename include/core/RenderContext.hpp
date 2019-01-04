#pragma once

#include <string>

struct NVGcontext;

namespace fui {

class RenderContext {
public:
  RenderContext();
  NVGcontext* vg() const { return _vg; }

  int loadFont(const std::string& name, const std::string& filename);
  int loadFont(const std::string& name, const void* fontData, int length);

  int loadImage(const std::string& filename);

protected:
  NVGcontext* _vg;
};

} // namespace fui
