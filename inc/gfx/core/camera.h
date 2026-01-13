#pragma once

#include "gfx/math/vec3.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Camera
{

public:

    Camera();
    Camera(const Vec3d position, const Vec3d rotation, const double z_near, const double z_far, const double fov);

    Matrix4x4d get_view_matrix() const;
    Matrix4x4d get_projection_matrix(const double aspect_ratio) const;

    Vec3d get_forward() const; 

    void set_position(const Vec3d pos);
    void set_fov(const double f);
    void set_fov_degrees(const double fov_degrees);
    void set_rotation(const Vec3d rot);
    void set_rotation_degrees(const Vec3d rot_deg);
    void set_z_near(const double zn);
    void set_z_far(const double zf);

    Vec3d get_position() const;
    double get_fov() const;
    Vec3d get_rotation() const;

private:

    Vec3d position;
    Vec3d rotation;
    double z_near;
    double z_far;
    double fov;
};

}
