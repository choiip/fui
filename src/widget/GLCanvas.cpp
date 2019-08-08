#include "widget/GLCanvas.hpp"
#include "GL/glad.h"

namespace fui {

GLCanvas::GLCanvas(WidgetContainer* parent)
: Canvas(parent)
{}

void GLCanvas::drawScene() {
  auto area = childArea(); 
  auto x = area.x;
  auto y = area.y;
  auto w = area.w;
  auto h = area.h;

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
