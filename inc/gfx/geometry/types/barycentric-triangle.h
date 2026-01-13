#pragma once

#include "gfx/math/vec2.h"
#include "gfx/math/vec3.h"
#include "gfx/math/box2.h"

namespace gfx
{


class BarycentricTriangle
{

public:

    BarycentricTriangle(const Vec2d& v0, const Vec2d& v1, const Vec2d& v2);
    BarycentricTriangle();

    const Vec2d v0;
    const Vec2d v1;
    const Vec2d v2;

    int corners_inside(const Box2d& box) const;
    bool point_inside(const Vec2d& point) const;
    double edge(const Vec2d& v_start, const Vec2d& v_end, const Vec2d& point) const;

    Vec3d barycentric_weights(const Vec2d& p) const;

    Vec3d lerp_barycentric(const Vec3d& a, const Vec3d& b, const Vec3d& c, const Vec2d& point) const;

    double get_area() const;

    double get_a() const;
    double get_b() const;
    double get_c() const;
    double get_d() const;
    double get_e() const;
    double get_f() const;
    double get_g() const;
    double get_h() const;
    double get_i() const;

private:

    double inv_area_2x;
    double a, b, c, d, e, f, g, h, i;
    double area;

    void precompute();
};

}
