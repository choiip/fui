#pragma once

#include "core/GraphicsProfile.hpp"

namespace fui {

class GlfwGLES2Profile 
: public GraphicsProfile {
public:
    virtual ~GlfwGLES2Profile() {}

    virtual GraphicsAPI api() const override { return GraphicsAPI::OPENGL_ES; }

    virtual void prepare() const override;
};

}
