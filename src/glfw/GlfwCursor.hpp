#pragma once

#include <vector>
#include "core/Cursor.hpp"

struct GLFWwindow;
struct GLFWcursor;

namespace fui {

class GlfwCursor : public Cursor {
public:
  explicit GlfwCursor(GLFWwindow* window);
  virtual ~GlfwCursor();

  virtual void hide() override;

  virtual void show() override;

  virtual Vector2i position() override;

  virtual void setShape(Shape shape) override;

private:
  GLFWwindow* _window;
  std::vector<GLFWcursor*> _cursors;
};

} // namespace fui
