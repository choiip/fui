#include "core/GlfwCursor.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace fui {

constexpr const int glfwShape[] = {
    GLFW_ARROW_CURSOR,     GLFW_IBEAM_CURSOR,
    GLFW_ARROW_CURSOR, // wait
    GLFW_CROSSHAIR_CURSOR,
    GLFW_ARROW_CURSOR, // waitarrow
    GLFW_ARROW_CURSOR, // sizenwse
    GLFW_ARROW_CURSOR, // sizenesw
    GLFW_HRESIZE_CURSOR,   GLFW_VRESIZE_CURSOR,
    GLFW_ARROW_CURSOR, // sizeall
    GLFW_ARROW_CURSOR, // no
    GLFW_HAND_CURSOR,
};

GlfwCursor::GlfwCursor(GLFWwindow* window)
: _window(window)
, _cursors(sizeof(glfwShape) / sizeof(int), nullptr) {
  auto itr = _cursors.begin();
  for (auto&& shape : glfwShape) {
    *itr = glfwCreateStandardCursor(shape);
    ++itr;
  }
}

GlfwCursor::~GlfwCursor() {
  for (auto&& c : _cursors) {
    glfwDestroyCursor(c);
  }
}

void GlfwCursor::hide() { glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); }

void GlfwCursor::show() { glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

void GlfwCursor::setShape(Shape shape) { glfwSetCursor(_window, _cursors[(int)shape]); }

} // namespace fui
