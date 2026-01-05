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

            Matrix4x1d clip = projection_matrix * view_matrix * model_matrix * pos_h;

            VertOutput out {
                .xyz = { clip(0,0), clip(1,0), clip(2,0) },
                .w = clip(3,0),
            };

            return out;
        }
    };

    class BasicFragShader : public Shader3D::FragShader
    {

        Color4 frag(const Shader3D::FragInput &input) const override
        {
            double depth = std::clamp(input.depth, 0.0, 1.0);

            double shade = depth;

            int intensity = (1 - shade) * 255;
            return Color4(255, 255, 255, intensity);
        }
    };
 
};

}
