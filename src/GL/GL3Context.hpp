#pragma once

#include "GL/OpenGLContext.hpp"

namespace fui {

enum class Status;

class GL3Context : public OpenGLContext {
public:
  GL3Context();
  virtual ~GL3Context();

  Status initVG();

protected:
  virtual std::string versionLine() const override;
  virtual std::string vertexMacro() const override;
  virtual std::string fragmentMacro() const override;
};

} // namespace fui
