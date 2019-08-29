#pragma once

#include "Widget.hpp"

struct NVGcontext;

namespace fui {

class PictureBox : public Widget {
public:
  explicit PictureBox(WidgetContainer* parent);

  virtual ~PictureBox();

  /// Centers the picture without affecting the scaling factor.
  auto center() -> decltype(this);

  /// Centers and scales the picture so that it fits inside the widgets.
  auto fit() -> decltype(this);

  virtual void draw(RenderContext& renderContext) override;

protected:
  FUI_WIDGET_PROPERTY_ALT_SETTER(int, picture, 0);

  FUI_WIDGET_READONLY_PROPERTY(Vector2i, pictureSize, {0, 0});

  FUI_WIDGET_PROPERTY(Vector2i, offset, {0, 0});
  FUI_WIDGET_PROPERTY(float, orientation, 0);
  FUI_WIDGET_PROPERTY_ALT_SETTER(float, scale, 1.f);

private:
  auto _pictureSetter(int const& v, NVGcontext* ctx) -> decltype(this);
  auto _pictureSetter(const std::string&, NVGcontext* ctx) -> decltype(this);
  auto _scaleSetter(float const& v) -> decltype(this);
};

} // namespace fui