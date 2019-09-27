#pragma once

#include "core/RenderContext.hpp"

namespace fui {

enum class Status;

class GLES3Context : public RenderContext {
public:
  GLES3Context();
  ~GLES3Context();

  Status initVG();

  virtual auto setViewport(int x, int y, int width, int height) -> decltype(this) override;
  virtual auto preDraw(const Recti& renderArea, const Color* clearColor = nullptr, const float* clearDepth = nullptr, const int* clearStencil = nullptr) -> decltype(this) override;
  virtual auto postDraw() -> decltype(this) override;

protected:
  virtual std::string versionLine() const override;
  virtual std::string vertexMacro() const override;
  virtual std::string fragmentMacro() const override;
};

} // namespace fui
