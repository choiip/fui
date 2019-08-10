#include "widget/GLCanvas.hpp"
#include "GL/glad.h"

namespace fui {

GLCanvas::GLCanvas(WidgetContainer* parent)
: Canvas(parent)
{}

void GLCanvas::drawScene() {
  auto viewport = mapTo({ 0, 0, _size.x, _size.y }, Coordinate::Framebuffer);

  auto x = viewport.x;
  auto y = viewport.y;
  auto w = viewport.w;
  auto h = viewport.h;

  GLint storedViewport[4];
  glGetIntegerv(GL_VIEWPORT, storedViewport);
  glViewport(x, y, w, h);
  glEnable(GL_SCISSOR_TEST);
  glScissor(x, y, w, h);
  _drawFunction();
  glDisable(GL_SCISSOR_TEST);
  glViewport(storedViewport[0], storedViewport[1],
            storedViewport[2], storedViewport[3]);
}

} // namespace fui
