#pragma once

#include "gfx/core/shader-3D.h"

namespace gfx
{
    class DiffuseFragShader final : public Shader3D::FragShader
    {

    public:

        std::vector<Color4> frag(const Shader3D::FragInput &input) const override
        {
            std::vector<Color4> out;
            out.reserve(input.normals.size());

            for (int i = 0; i < input.normals.size(); i++)
            {
                const double diffuse = std::max(0.0, Vec3d::dot(input.normals[i], input.light_dir));
                const double intensity = input.ambient_intensity + (1.0 - input.ambient_intensity) * diffuse;

                out.emplace_back(
                    intensity * input.colors[i].r_double(),
                    intensity * input.colors[i].g_double(),
                    intensity * input.colors[i].b_double(),
                    1.0
                );
            }

            return out;
        }
    };
}
