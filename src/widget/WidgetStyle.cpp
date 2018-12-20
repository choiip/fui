#include "widget/WidgetStyle.hpp"
#include "nanovg/nanovg.h"
#include "text/FontDescriptor.hpp"

namespace fui {

WidgetStyle::WidgetStyle(NVGcontext* vg) 
: _vg(vg) 
{
  FontDescriptor fontDescriptor;
  auto resultFontDescriptor = findFont(&fontDescriptor);
  
	fontNormal = nvgCreateFont(vg, "sans", "examples/assets/fonts/Roboto-Regular.ttf");
	if (fontNormal == -1) {
		printf("Could not add font italic.\n");
	}
	fontBold = nvgCreateFont(vg, "sans-bold", "examples/assets/fonts/Roboto-Bold.ttf");
	if (fontBold == -1) {
		printf("Could not add font bold.\n");
	}  
  fontIcons = nvgCreateFont(vg, "icons", "examples/assets/fonts/entypo.ttf");
	if (fontIcons == -1) {
		printf("Could not add font icons.\n");
	}
}

}
