#pragma once

#include <cmath>

#include "gfx/math/vec2.h"
#include "gfx/math/box2.h"

namespace gfx
{

class Triangle
{

public:

    Triangle(const Vec2d& v0, const Vec2d& v1, const Vec2d& v2)
        : v0(v0), v1(v1), v2(v2)
    {
        precompute();
    }

    Triangle()
        : v0(Vec2d::zero()), v1(Vec2d::zero()), v2(Vec2d::zero())
    {
        precompute();
    }

    const Vec2d v0;
    const Vec2d v1;
    const Vec2d v2;

    int corners_inside(const Box2d& box) const
    {
        Vec2d box_corners[4] = {
            { box.min.x, box.min.y },
            { box.max.x, box.min.y },
            { box.max.x, box.max.y },
            { box.min.x, box.max.y }
        };

        int count = 0;

        for (const auto& corner : box_corners)
        {
            if (point_inside(corner))
            {
                count++;
            }
        }

        return count;
    }

    bool point_inside(const Vec2d& point) const
    {
        double s = inv_area_2x * (a * point.x + b * point.y + c);
        double t = inv_area_2x * (d * point.x + e * point.y + f);

        return s >= 0 && t >= 0 && (s + t) <= 1;
    }

    double edge(const Vec2d& v_start, const Vec2d& v_end, const Vec2d& point) const
    {
        return (v_end.x - v_start.x) * (point.y - v_start.y) - (v_end.y - v_start.y) * (point.x - v_start.x);
    }

    double get_area() const { return area; }

    double get_a() const { return a; }
    double get_b() const { return b; }
    double get_c() const { return c; }
    double get_d() const { return d; }
    double get_e() const { return e; }
    double get_f() const { return f; }
    double get_g() const { return g; }
    double get_h() const { return h; }
    double get_i() const { return i; }

private:

    double inv_area_2x;
    double a, b, c, d, e, f, g, h, i;
    double area;

    void precompute()
    {
        double area_2x = v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y);
        area = area_2x / 2.0;

        if (std::abs(area_2x) < 1e-12)
        {
            inv_area_2x = 0.0;
        }
        else
        {
            inv_area_2x = 1.0 / area_2x;
        }

        a = v2.y - v0.y;
        b = v0.x - v2.x;
        c = v0.y * v2.x - v0.x * v2.y;

        d = v0.y - v1.y;
        e = v1.x - v0.x;
        f = v0.x * v1.y - v0.y * v1.x;

        g = v1.y - v2.y;
        h = v2.x - v1.x;
        i = v1.x * v2.y - v1.y * v2.x;
    }
};

}
