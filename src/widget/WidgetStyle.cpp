#include "widget/WidgetStyle.hpp"
#include <memory>
#include "nanovg/nanovg.h"
#include "text/FontDescriptor.hpp"

namespace fui {

WidgetStyle::WidgetStyle(NVGcontext* vg) 
: _vg(vg) 
{
  FontDescriptor requestFont;
  requestFont.lang = "zh-hk";
  requestFont.style = "Regular";
  auto resultFontDescriptor = std::unique_ptr<FontDescriptor>(findFont(&requestFont));
  
	fontNormal = nvgCreateFont(vg, "sans", resultFontDescriptor->path.c_str());
	if (fontNormal == -1) {
		printf("Could not add font italic.\n");
	}
  requestFont.style = "Bold";
  resultFontDescriptor = std::unique_ptr<FontDescriptor>(findFont(&requestFont));

	fontBold = nvgCreateFont(vg, "sans-bold", resultFontDescriptor->path.c_str());
	if (fontBold == -1) {
		printf("Could not add font bold.\n");
	}  
  fontIcons = nvgCreateFont(vg, "icons", "examples/assets/fonts/entypo.ttf");
	if (fontIcons == -1) {
		printf("Could not add font icons.\n");
	}
}

}
