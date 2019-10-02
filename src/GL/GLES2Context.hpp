#pragma once

#include "GL/OpenGLContext.hpp"

namespace fui {

enum class Status;

class GLES2Context : public OpenGLContext {
public:
  GLES2Context();
  virtual ~GLES2Context();

  Status initVG();

protected:
  virtual std::string versionLine() const override;
  virtual std::string vertexMacro() const override;
  virtual std::string fragmentMacro() const override;
};

} // namespace fui
