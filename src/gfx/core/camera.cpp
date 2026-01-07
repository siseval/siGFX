#include "gfx/core/camera.h"

#include "gfx/geometry/transform-3D.h"

namespace gfx
{

Matrix4x4d Camera::get_view_matrix() const
{
    Matrix4x4d translation = Transform3D::translate({ -position.x, -position.y, -position.z });
    Matrix4x4d rotation_x = Transform3D::rotate_x(-rotation.x);
    Matrix4x4d rotation_y = Transform3D::rotate_y(-rotation.y);
    Matrix4x4d rotation_z = Transform3D::rotate_z(-rotation.z);

    return rotation_x * rotation_y * rotation_z * translation;
}

Matrix4x4d Camera::get_projection_matrix(const double aspect_ratio) const
{
    double fov_rad { fov * (std::numbers::pi / 180.0) };
    double f { 1.0 / std::tan(fov_rad / 2.0) };

    Matrix4x4d projection { Matrix4x4d::zero() };
    projection(0, 0) = f / aspect_ratio;
    projection(1, 1) = f;

    projection(2, 2) = z_far / (z_far - z_near);
    projection(2, 3) = -z_near * z_far / (z_far - z_near);
    projection(3, 2) = 1.0;

    return projection;
}

Vec3d Camera::get_forward_vector() const
{
    double pitch_rad { rotation.x };
    double yaw_rad { rotation.y };

    Vec3d forward {
        std::cos(pitch_rad) * std::sin(yaw_rad),
        std::sin(pitch_rad),
        std::cos(pitch_rad) * std::cos(yaw_rad)
    };

    return forward.normalize();
}

}
