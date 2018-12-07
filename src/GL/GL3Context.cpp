#include "GL/GL3Context.hpp"
#include <functional>
#include <iostream>

#include "GL/glad.h"

#include "nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include "core/Status.hpp"
#include "core/RenderWindow.hpp"

namespace fui {

Status GL3Context::init(RenderWindow* window) {
    if (window == nullptr) {
        return Status::INVALID_ARGUMENT;
    }

    _window = window;

    _vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (_vg == NULL) {
		std::cerr << ("Could not init nanovg.\n");
		return Status::UNKNOWN_ERROR;
	}

    return Status::OK;
}

void GL3Context::deinit() {
	nvgDeleteGL3(_vg);
     _window = nullptr;
}

NVGcontext* GL3Context::vg() const {
    return _vg;
}

}
