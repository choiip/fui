#pragma once

#include "core/RenderContext.hpp"

namespace fui {

class OpenGLContext : public RenderContext {
public:
  OpenGLContext();
  virtual ~OpenGLContext();

  std::string loadVertexShader(const std::string& filename);
  std::string loadFragmentShader(const std::string& filename);

  virtual auto setViewport(int x, int y, int width, int height) -> decltype(this) override;
  virtual auto preDraw(const Recti& renderArea, const Color* clearColor = nullptr, const float* clearDepth = nullptr,
                       const int* clearStencil = nullptr) -> decltype(this) override;
  virtual auto postDraw() -> decltype(this) override;

protected:
  virtual std::string versionLine() const = 0;
  virtual std::string vertexMacro() const = 0;
  virtual std::string fragmentMacro() const = 0;
};

} // namespace fui
