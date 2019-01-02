#include "core/RenderContext.hpp"
#include "nanovg/nanovg.h"

namespace fui {

int RenderContext::loadFont(const std::string& name, const std::string& filename) {
  return nvgCreateFont(_vg, name.c_str(), filename.c_str());
}

int RenderContext::loadFont(const std::string& name, const void* fontData, int length) {
  return nvgCreateFontMem(_vg, name.c_str(), (unsigned char*)fontData, length, 0);
}

} // namespace fui
