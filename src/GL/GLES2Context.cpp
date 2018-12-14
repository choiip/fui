#include "GL/GLES2Context.hpp"
#include <GL/glad.h>
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"

namespace fui {

GLES2Context::GLES2Context(NVGcontext* vg)
: RenderContext(vg) {}

GLES2Context::~GLES2Context() { nvgDeleteGLES2(vg()); }

} // namespace fui
