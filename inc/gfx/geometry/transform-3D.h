#pragma once

#include "gfx/math/vec3.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Transform3D
{

public:

    static Matrix4x4d translate(const Vec3d pos);
    static Matrix4x4d rotate_x(const double angle);
    static Matrix4x4d rotate_y(const double angle);
    static Matrix4x4d rotate_z(const double angle);
    static Matrix4x4d scale(const Vec3d scale);

    static Vec3d extract_translation(const Matrix4x4d &transform);
    static double extract_rotation(const Matrix4x4d &transform);
    static Vec3d extract_scale(const Matrix4x4d &transform);

    static Vec3d transform_point(const Vec3d pos, const Matrix4x4d &transform);
    static Vec3d transform_vector(const Vec3d vec, const Matrix4x4d &transform);

    static std::vector<Vec3d> transform_points(const std::vector<Vec3d> points, const Matrix4x4d &transform);
    static std::vector<Vec3d> transform_vectors(const std::vector<Vec3d> vectors, const Matrix4x4d &transform);

    static Matrix4x4d invert_affine(const Matrix4x4d &m);


};

}

