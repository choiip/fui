#pragma once

struct NVGcontext;

namespace fui {

class RenderContext {
public:
  RenderContext()
  : _vg(nullptr) {}
  NVGcontext* vg() const { return _vg; }

protected:
  NVGcontext* _vg;
};

} // namespace fui
