#pragma once

namespace fui {

enum class GraphicsAPI {
    OPENGL,
    OPENGL_ES,
    VULKAN,
};

class GraphicsProfile {
public:
    virtual ~GraphicsProfile() {}
    virtual GraphicsAPI api() const = 0;
    virtual void prepare() const = 0;
};

}
