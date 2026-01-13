#pragma once

#include "gfx/core/shader-3D.h"

namespace gfx
{

class DiffuseFragShader : public Shader3D::FragShader
{

public:

    std::vector<Color4> frag(const Shader3D::FragInput &input) const override
    {
        std::vector<Color4> out;
        out.reserve(input.normals.size());
        
        for (int i = 0; i < input.normals.size(); i++)
        {
            double diffuse = std::max(0.0, Vec3d::dot(input.normals[i], input.light_dir));
            double intensity = input.ambient_intensity + (1.0 - input.ambient_intensity) * diffuse;

            out.emplace_back(intensity, intensity, intensity, 1.0);
        }

        return out;
    }
};


}
