#pragma once

#include "gfx/math/matrix.h"
#include "gfx/math/vec3.h"

namespace gfx
{
    class Camera
    {

    public:

        Camera();
        Camera(Vec3d position, Vec3d rotation, double z_near, double z_far, double fov);

        Matrix4x4d get_view_matrix() const;
        Matrix4x4d get_projection_matrix(double aspect_ratio) const;

        Vec3d get_forward() const;

        void set_position(Vec3d pos);
        void set_fov(double f);
        void set_fov_degrees(double fov_degrees);
        void set_rotation(Vec3d rot);
        void set_rotation_degrees(Vec3d rot_deg);
        void set_z_near(double zn);
        void set_z_far(double zf);

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
