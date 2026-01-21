#pragma once

#include "gfx/core/shader-3D.h"

namespace gfx
{
class DiffuseFragShader final : public Shader3D::FragShader
{

public:

    Color4 frag(const Shader3D::FragInput &input) const override
    {
        const double diffuse = std::max(0.0, Vec3d::dot(input.normal, uniforms.light_dir));
        const double intensity = uniforms.ambient_intensity + (1.0 - uniforms.ambient_intensity) * diffuse;

        return Color4(
            intensity * input.color.r_double(),
            intensity * input.color.g_double(),
            intensity * input.color.b_double(),
            1.0
        );
    }
};
}
