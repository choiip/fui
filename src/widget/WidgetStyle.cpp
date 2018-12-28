#include "widget/WidgetStyle.hpp"
#include <memory>
#include "builtin/EntypoTFF.h"
#include "core/Log.hpp"
#include "nanovg/nanovg.h"
#include "text/FontDescriptor.hpp"

namespace fui {

WidgetStyle::WidgetStyle(NVGcontext* vg)
: _vg(vg) {
  // build in fonts
  fontIcons = nvgCreateFontMem(vg, "icons", (unsigned char*)EntypoTFF, sizeof(EntypoTFF), 0);
  if (fontIcons == -1) {
    LOGE << "Could not add font icons.";
  }

  FontDescriptor requestFont;
  requestFont.lang = "zh-hk";
  requestFont.style = "Regular";
  auto resultFontDescriptor = std::unique_ptr<FontDescriptor>(findFont(&requestFont));

  fontStandard = nvgCreateFont(vg, "sans", resultFontDescriptor->path.c_str());
  if (fontStandard == -1) {
    LOGE << "Could not add standard font.";
  }
  requestFont.style = "Bold";
  resultFontDescriptor = std::unique_ptr<FontDescriptor>(findFont(&requestFont));

  fontBold = nvgCreateFont(vg, "sans-bold", resultFontDescriptor->path.c_str());
  if (fontBold == -1) {
    LOGE << "Could not add bold font.";
  }
}

} // namespace fui
