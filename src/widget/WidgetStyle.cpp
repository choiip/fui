#include "widget/WidgetStyle.hpp"
#include <memory>
#include "builtin/EntypoTFF.h"
#include "core/Log.hpp"
#include "core/RenderContext.hpp"
#include "text/FontDescriptor.hpp"

namespace fui {

WidgetStyle::WidgetStyle(RenderContext& renderContext) {
  // build in fonts
  fontIcons = renderContext.loadFont("icons", (unsigned char*)EntypoTFF, sizeof(EntypoTFF));
  if (fontIcons == -1) {
    LOGE << "Could not add font icons.";
  }
#ifndef __EMSCRIPTEN__
  FontDescriptor requestFont;
  requestFont.lang = "zh-hk";
  requestFont.style = "Regular";
  auto resultFontDescriptor = std::unique_ptr<FontDescriptor>(findFont(&requestFont));

  fontStandard = renderContext.loadFont("sans", resultFontDescriptor->path);
  if (fontStandard == -1) {
    LOGE << "Could not add standard font.";
  }
  requestFont.style = "Bold";
  resultFontDescriptor = std::unique_ptr<FontDescriptor>(findFont(&requestFont));

  fontBold = renderContext.loadFont("sans-bold", resultFontDescriptor->path);
  if (fontBold == -1) {
    LOGE << "Could not add bold font.";
  }
#endif  
}

} // namespace fui
