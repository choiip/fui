#pragma once

struct NVGcontext;

namespace fui {

class RenderContext {
public:
  explicit RenderContext(NVGcontext* vg)
  : _vg(vg) {}
  NVGcontext* vg() const { return _vg; }

private:
  NVGcontext* _vg = nullptr;
};

} // namespace fui
