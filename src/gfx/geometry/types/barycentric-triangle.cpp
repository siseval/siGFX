#include "gfx/geometry/types/barycentric-triangle.h"

namespace gfx
{

    BarycentricTriangle::BarycentricTriangle(const Vec2d &v0, const Vec2d &v1, const Vec2d &v2)
        : v0(v0), v1(v1), v2(v2)
    {
        precompute();
    }

    BarycentricTriangle::BarycentricTriangle()
        : v0(Vec2d::zero()), v1(Vec2d::zero()), v2(Vec2d::zero())
    {
        precompute();
    }

    int BarycentricTriangle::corners_inside(const Box2d &box) const
    {
        Vec2d box_corners[4] = {
            { box.min.x, box.min.y },
            { box.max.x, box.min.y },
            { box.max.x, box.max.y },
            { box.min.x, box.max.y }
        };

        int count = 0;

        for (const auto &corner : box_corners)
        {
            if (point_inside(corner))
            {
                count++;
            }
        }

        return count;
    }

    bool BarycentricTriangle::point_inside(const Vec2d &point) const
    {
        const double s = inv_area_2x * (a * point.x + b * point.y + c);
        const double t = inv_area_2x * (d * point.x + e * point.y + f);

        return s >= 0 && t >= 0 && s + t <= 1;
    }

    double BarycentricTriangle::edge(const Vec2d &v_start, const Vec2d &v_end, const Vec2d &point)
    {
        return (v_end.x - v_start.x) * (point.y - v_start.y) - (v_end.y - v_start.y) * (point.x - v_start.x);
    }

    Vec3d BarycentricTriangle::barycentric_weights(const Vec2d &p) const
    {
        double w1 = inv_area_2x * (a * p.x + b * p.y + c);
        double w2 = inv_area_2x * (d * p.x + e * p.y + f);
        double w0 = 1.0 - w1 - w2;
        return { w0, w1, w2 };
    }

    Vec3d BarycentricTriangle::lerp_barycentric(
        const Vec3d &a,
        const Vec3d &b,
        const Vec3d &c,
        const Vec2d &point
    ) const
    {
        const Vec3d w = barycentric_weights(point);
        return a * w.x + b * w.y + c * w.z;
    }

    double BarycentricTriangle::get_area() const
    {
        return area;
    }

    double BarycentricTriangle::get_a() const
    {
        return a;
    }

    double BarycentricTriangle::get_b() const
    {
        return b;
    }

    double BarycentricTriangle::get_c() const
    {
        return c;
    }

    double BarycentricTriangle::get_d() const
    {
        return d;
    }

    double BarycentricTriangle::get_e() const
    {
        return e;
    }

    double BarycentricTriangle::get_f() const
    {
        return f;
    }

    double BarycentricTriangle::get_g() const
    {
        return g;
    }

    double BarycentricTriangle::get_h() const
    {
        return h;
    }

    double BarycentricTriangle::get_i() const
    {
        return i;
    }

    void BarycentricTriangle::precompute()
    {
        const double area_2x = v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y);
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

}
