#include "GL/GL3Context.hpp"
#include <GL/glad.h>
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

namespace fui {

GL3Context::GL3Context(NVGcontext* vg)
: RenderContext(vg)
{}

GL3Context::~GL3Context() {
	nvgDeleteGL3(vg());
}

}
