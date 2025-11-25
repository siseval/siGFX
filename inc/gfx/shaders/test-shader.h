#ifndef TEST_SHADER_H
#define TEST_SHADER_H

#include <algorithm>
#include <gfx/core/shader-2D.h>

namespace gfx::shaders
{

class TestShader : public gfx::core::Shader2D<TestShader>
{

public:

    gfx::core::types::Color4 frag(const gfx::core::ShaderInput2D &input) const
    {
        int r = static_cast<int>(std::clamp(input.uv.x * 255.0, 0.0, 255.0));
        int g = static_cast<int>(std::clamp(input.uv.y * 255.0, 0.0, 255.0));
        int b = static_cast<int>(std::clamp((0.5 + 0.5 * std::sin(input.t)) * 255.0, 0.0, 255.0));
        return gfx::core::types::Color4(r, g, b, 255);
    }

};

}

#endif // TEST_SHADER_H
