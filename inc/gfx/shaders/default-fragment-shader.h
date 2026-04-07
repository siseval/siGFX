#pragma once

#include "gfx/core/material/fragment-shader.h"

namespace gfx
{
class DefaultFragmentShader final : public FragmentShader
{

public:

    Color4 frag(const FragmentShader::Input &input, const FragmentShader::Uniforms &uniforms) const override
    {
        return input.color;
    }
};

}
