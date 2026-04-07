#pragma once

#include "gfx/math/vec3.h"
#include "gfx/core/types/color4.h"

namespace gfx
{

class FragmentShader
{

public:

    struct Uniforms
    {
        double t;
        Vec3d light_dir;
        double ambient_intensity;
        double near_plane;
        double far_plane;
    };

    struct Input
    {
        Vec3d uvw;
        double depth;
        Vec3d normal;
        Color4 color;
    };

    virtual ~FragmentShader() = default;

    virtual Color4 frag(const Input &input, const Uniforms &uniforms) const = 0;

protected:

};

}
