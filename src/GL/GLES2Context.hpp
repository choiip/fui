#pragma once

#include "core/RenderContext.hpp"

namespace fui {

enum class Status;

class GLES2Context : public RenderContext {
public:
  GLES2Context();
  ~GLES2Context();

  Status initVG();

  virtual auto setViewport(int x, int y, int width, int height) -> decltype(this) override;
  
protected:
  virtual std::string versionLine() const override;
  virtual std::string vertexMacro() const override;
  virtual std::string fragmentMacro() const override;
};

} // namespace fui
