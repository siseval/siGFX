#pragma once

#include "gfx/core/shader-3D.h"
#include "gfx/shaders/default-frag-shader.h"
#include "gfx/shaders/default-vert-shader.h"

namespace gfx
{
    class DefaultShader3D : public Shader3D
    {

    public:

        DefaultShader3D()
            : Shader3D(
                std::make_shared<DefaultVertShader>(),
                std::make_shared<DefaultFragShader>()
            ) {}
    };
}
