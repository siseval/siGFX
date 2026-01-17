#include "gfx/core/camera.h"

#include "gfx/geometry/transform-3D.h"

namespace gfx
{

    Camera::Camera()
        : position { 0.0, 0.0, 0.0 },
          rotation { 0.0, 0.0, 0.0 },
          z_near { 0.1 },
          z_far { 1000.0 },
          fov { std::numbers::pi / 4.0 } {}

    Camera::Camera(
        const Vec3d position,
        const Vec3d rotation,
        const double z_near,
        const double z_far,
        const double fov
    )
        : position { position },
          rotation { rotation },
          z_near { z_near },
          z_far { z_far },
          fov { fov } {}

    void Camera::set_position(const Vec3d pos)
    {
        position = pos;
    }

    Vec3d Camera::get_position() const
    {
        return position;
    }

    void Camera::set_fov(const double f)
    {
        fov = f;
    }

    void Camera::set_fov_degrees(const double fov_degrees)
    {
        fov = fov_degrees * std::numbers::pi / 180;
    }

    double Camera::get_fov() const
    {
        return fov;
    }

    void Camera::set_rotation(const Vec3d rot)
    {
        rotation = rot;
    }

    void Camera::set_rotation_degrees(const Vec3d rot_deg)
    {
        rotation = Vec3d {
            rot_deg.x * std::numbers::pi / 180,
            rot_deg.y * std::numbers::pi / 180,
            rot_deg.z * std::numbers::pi / 180
        };
    }

    Vec3d Camera::get_rotation() const
    {
        return rotation;
    }

    void Camera::set_z_near(const double zn)
    {
        z_near = zn;
    }

    void Camera::set_z_far(const double zf)
    {
        z_far = zf;
    }

    Matrix4x4d Camera::get_view_matrix() const
    {
        const Matrix4x4d translation = Transform3D::translate({ -position.x, -position.y, -position.z });
        const Matrix4x4d rotation_x = Transform3D::rotate_x(-rotation.x);
        const Matrix4x4d rotation_y = Transform3D::rotate_y(-rotation.y);
        const Matrix4x4d rotation_z = Transform3D::rotate_z(-rotation.z);

        return rotation_x * rotation_y * rotation_z * translation;
    }

    Matrix4x4d Camera::get_projection_matrix(const double aspect_ratio) const
    {
        const double f { 1.0 / std::tan(fov / 2.0) };

        Matrix4x4d projection { Matrix4x4d::zero() };
        projection(0, 0) = f / aspect_ratio;
        projection(1, 1) = f;

        projection(2, 2) = z_far / (z_far - z_near);
        projection(2, 3) = -z_near * z_far / (z_far - z_near);
        projection(3, 2) = 1.0;

        return projection;
    }

    Vec3d Camera::get_forward() const
    {
        const double pitch_rad { rotation.x };
        const double yaw_rad { rotation.y };

        const Vec3d forward {
            std::cos(pitch_rad) * std::sin(yaw_rad),
            std::sin(pitch_rad),
            std::cos(pitch_rad) * std::cos(yaw_rad)
        };

        return forward.normalize();
    }

}
