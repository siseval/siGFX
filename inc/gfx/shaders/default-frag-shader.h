#pragma once

#include "gfx/core/shader-3D.h"

namespace gfx
{
    class DefaultFragShader final : public Shader3D::FragShader
    {

    public:

        std::vector<Color4> frag(const Shader3D::FragInput &input) const override
        {
            std::vector<Color4> out;

            for (size_t i = 0; i < input.uvw.size(); ++i)
            {
                out.push_back(input.colors[i]);
            }

            return out;
        }
    };
}
