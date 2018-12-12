#pragma once 

#include "core/RenderContext.hpp"

namespace fui {

class RenderWindow;
enum class Status;

class GL3Context
: public RenderContext {
public:
    GL3Context(NVGcontext* vg);
    ~GL3Context();
};

}
