#pragma once

struct NVGcontext;

namespace fui {

class WidgetStyle {
public:
  WidgetStyle(NVGcontext* vg);

  /// Fonts ///
  int fontNormal;
  int fontBold;
  int fontIcons;

  /// Button ///
  float buttonCornerRadius = 4.f;
  
private:
  NVGcontext* _vg;
};

}
