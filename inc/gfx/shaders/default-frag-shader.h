#pragma once

#include "gfx/core/shader-3D.h"

namespace gfx
{
class DefaultFragShader final : public Shader3D::FragShader
{

public:

    Color4 frag(const Shader3D::FragInput &input) const override
    {
        return input.color;
    }
};

}
