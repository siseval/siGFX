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
            out.xyz.reserve(input.positions.size());
            out.w.reserve(input.positions.size());
            out.normals.reserve(input.normals.size());

            for (int i = 0; i < input.positions.size(); i++)
            {
                Matrix4x1d pos_h {
                    { input.positions[i].x },
                    { input.positions[i].y },
                    { input.positions[i].z },
                    { 1.0 }
                };

                Matrix4x1d pos_clip = input.mvp_matrix * pos_h;

                out.xyz[i] = { pos_clip(0, 0), pos_clip(1, 0), pos_clip(2, 0) };
                out.w[i] = pos_clip(3, 0);
            }

            for (int i = 0; i < input.normals.size(); i++)
            {
                Matrix4x1d normal_h {
                    { input.normals[i].x },
                    { input.normals[i].y },
                    { input.normals[i].z },
                    { 0.0 }
                };

                Matrix4x1d normal_clip = input.model_matrix * normal_h;
                out.normals[i] = { normal_clip(0, 0), normal_clip(1, 0), normal_clip(2, 0) };
            }

            return out;
        }
    };
}
