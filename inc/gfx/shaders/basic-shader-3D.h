#pragma once

#include "gfx/core/shader-3D.h"

namespace gfx
{

class BasicShader3D : public Shader3D
{

public:

    BasicShader3D()
    {
        vertex_shader = std::make_shared<BasicVertShader>();
        fragment_shader = std::make_shared<BasicFragShader>();
    }

private:

    class BasicVertShader : public Shader3D::VertShader
    {

    public:

        VertOutput vert(const Shader3D::VertInput& input) const override
        {
            Matrix4x1d pos_h {
                { input.pos.x },
                { input.pos.y },
                { input.pos.z },
                { 1.0 }
            };

            Matrix4x1d normal_h {
                { input.normal.x },
                { input.normal.y },
                { input.normal.z },
                { 0.0 }
            };

            Matrix4x1d pos_clip = projection_matrix * view_matrix * model_matrix * pos_h;
            Matrix4x1d normal_clip = model_matrix * normal_h;

            VertOutput out {
                .xyz = { pos_clip(0,0), pos_clip(1,0), pos_clip(2,0) },
                .w = pos_clip(3,0),
                .normal = { normal_clip(0,0), normal_clip(1,0), normal_clip(2,0) }
            };

            return out;
        }
    };

    class BasicFragShader : public Shader3D::FragShader
    {

        Color4 frag(const Shader3D::FragInput &input) const override
        {
            double diffuse = std::max(0.0, Vec3d::dot(input.normal, light_dir));
            double ambient = 0.1;
            double intensity = ambient + (1.0 - ambient) * diffuse;

            Vec3d rgb { input.normal * 0.5 };

            return Color4(intensity, intensity, intensity, 1.0);
        }
    };
 
};

}
