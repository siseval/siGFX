#pragma once

#include <algorithm>

#include "gfx/core/shader-2D.h"

namespace gfx
{

class TestShader : public Shader2D
{

public:

    Color4 frag(const ShaderInput2D &input) const override
    {
        int r = static_cast<int>(std::clamp(input.uv.x * 255.0, 0.0, 255.0));
        int g = static_cast<int>(std::clamp(input.uv.y * 255.0, 0.0, 255.0));
        int b = static_cast<int>(std::clamp((0.5 + 0.5 * std::sin(input.t)) * 255.0, 0.0, 255.0));
        return Color4(r, g, b, 255);
    }

};

}
