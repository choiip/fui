#pragma once 

struct NVGcontext;

namespace fui {

class RenderWindow;
enum class Status;

class GL3Context {
public:
    Status init(RenderWindow* window);
    void deinit();

    NVGcontext* vg() const;
private:
    RenderWindow* _window = nullptr;
    NVGcontext* _vg = nullptr;

};

}
