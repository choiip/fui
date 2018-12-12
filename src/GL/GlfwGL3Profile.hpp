#pragma once

#include "core/GraphicsProfile.hpp"

namespace fui {

class GlfwGL3Profile 
: public GraphicsProfile {
public:
    virtual ~GlfwGL3Profile() {}

    virtual GraphicsAPI api() const override { return GraphicsAPI::OPENGL; }

    virtual void prepare() const override;

    virtual RenderContext* createContext(void* nativeWindow) const override;
};

}
