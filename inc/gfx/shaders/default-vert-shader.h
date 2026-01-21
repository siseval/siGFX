#pragma once

#include "gfx/core/shader-3D.h"

namespace gfx
{
class DefaultVertShader final : public Shader3D::VertShader
{

public:

    Shader3D::VertOutput vert(const Shader3D::VertInput &input) const override
    {
        Shader3D::VertOutput out;

        Matrix4x1d pos_h {
            { input.pos.x },
            { input.pos.y },
            { input.pos.z },
            { 1.0 }
        };

        Matrix4x1d pos_clip = uniforms.mvp_matrix * pos_h;

        out.xyz = { pos_clip(0, 0), pos_clip(1, 0), pos_clip(2, 0) };
        out.w = pos_clip(3, 0);

        Matrix4x1d normal_h {
            { input.normal.x },
            { input.normal.y },
            { input.normal.z },
            { 0.0 }
        };

        Matrix4x1d normal_clip = uniforms.model_matrix * normal_h;
        out.normal = { normal_clip(0, 0), normal_clip(1, 0), normal_clip(2, 0) };

        return out;
    }
};
}
