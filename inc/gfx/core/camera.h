#pragma once

#include "gfx/math/vec3.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Camera
{

public:

    Matrix4x4d get_view_matrix() const;
    Matrix4x4d get_projection_matrix(const double aspect_ratio) const;

    Vec3d get_forward_vector() const; 

    inline void set_position(const Vec3d pos)
    {
        position = pos;
    }

    inline Vec3d get_position() const
    {
        return position;
    }

    inline void set_fov(const double f)
    {
        fov = f;
    }

    inline double get_fov() const
    {
        return fov;
    }

    inline void set_rotation(const Vec3d rot)
    {
        rotation = rot;
    }

    inline Vec3d get_rotation() const
    {
        return rotation;
    }

    inline void set_z_near(const double zn)
    {
        z_near = zn;
    }

    inline void set_z_far(const double zf)
    {
        z_far = zf;
    }


private:

    Vec3d position;
    Vec3d rotation;
    double z_near = 0.1;
    double z_far = 4.0;
    double fov = 60.0;
};

}
