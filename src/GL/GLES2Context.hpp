#pragma once 

#include "core/RenderContext.hpp"

namespace fui {

class RenderWindow;
enum class Status;

class GLES2Context
: public RenderContext {
public:
    GLES2Context(NVGcontext* vg);
    ~GLES2Context();
};

}
