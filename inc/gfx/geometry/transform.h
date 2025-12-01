#pragma once

#include "gfx/math/vec2.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Transform
{

public:

    static Matrix3x3d translate(const Vec2d pos);
    static Matrix3x3d rotate(const double angle);
    static Matrix3x3d scale(const Vec2d scale);

    static Vec2d extract_translation(const Matrix3x3d &transform);
    static double extract_rotation(const Matrix3x3d &transform);
    static Vec2d extract_scale(const Matrix3x3d &transform);

    static Vec2d transform_point(const Vec2d pos, const Matrix3x3d &transform);
    static Vec2d transform_vector(const Vec2d vec, const Matrix3x3d &transform);

    static std::vector<Vec2d> transform_points(const std::vector<Vec2d> points, const Matrix3x3d &transform);
    static std::vector<Vec2d> transform_vectors(const std::vector<Vec2d> vectors, const Matrix3x3d &transform);

    static Matrix3x3d invert_affine(const Matrix3x3d &m);


};

}

