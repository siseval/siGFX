#pragma once

#include "gfx/math/vec2.h"

namespace gfx
{

class OBB2D
{

public:

    OBB2D() : origin { 0, 0 }, side_x { 1, 0 }, side_y { 0, 1 } {}
    OBB2D(const Vec2d origin, const Vec2d side_x, const Vec2d side_y) : origin(origin), side_x(side_x), side_y(side_y) {}
    OBB2D(const OBB2D &other) : origin(other.origin), side_x(other.side_x), side_y(other.side_y) {}

    Vec2d origin;
    Vec2d side_x;
    Vec2d side_y;

    Vec2d get_center() const
    {
        return origin + (side_x * 0.5) + (side_y * 0.5);
    }

    Vec2d get_extent() const
    {
        return Vec2d { side_x.length(), side_y.length() };
    }

    Vec2d get_uv(const Vec2d point) const
    {
        Vec2d d { point - origin };
        double u { Vec2d::dot(d, side_x) / Vec2d::dot(side_x, side_x) };
        double v { Vec2d::dot(d, side_y) / Vec2d::dot(side_y, side_y) };
        return Vec2d { u, v };
    }

    std::vector<Vec2d> get_corners() const
    {
        return {
            origin,
            origin + side_x,
            origin + side_x + side_y,
            origin + side_y
        };
    }

};

}
