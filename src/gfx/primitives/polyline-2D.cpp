#include "gfx/primitives/polyline-2D.h"
#include "gfx/geometry/transform-2D.h"

namespace gfx
{

Box2d Polyline2D::get_geometry_size() const
{
    Box2d bounds { Vec2d::zero(), Vec2d::zero() };

    for (auto point : points)
    {
        bounds.expand(point);
    }

    return bounds;
}

Box2d Polyline2D::get_axis_aligned_bounding_box(const Matrix3x3d &transform) const
{
    Box2d extent { get_geometry_size() };
    Vec2d line_extent { std::ceil(line_thickness / 2.0), std::ceil(line_thickness / 2.0) };
    Vec2d top_left { extent.min - line_extent };
    Vec2d bot_right { extent.max + line_extent };

    std::vector<Vec2d> corners {
        { top_left.x, top_left.y },
        { bot_right.x, top_left.y },
        { top_left.x, bot_right.y },
        { bot_right.x, bot_right.y },
    };
    std::vector<Vec2d> transformed_corners { Transform2D::transform_points(corners, transform) };

    Box2d bounds { transformed_corners[0], transformed_corners[0] };
    bounds.expand(transformed_corners);

    return bounds;
}

bool Polyline2D::point_collides(const Vec2d point, const Matrix3x3d &transform) const
{
    return false;
}

bool Polyline2D::cache_clockwise()
{
    double sum = 0.0;
    for (int i = 0; i < points.size(); ++i)
    {
        Vec2d p0 { points[i] };
        Vec2d p1 { points[(i + 1) % points.size()] };
        sum += (p1.x - p0.x) * (p1.y + p0.y);
    }
    clockwise = sum < 0.0;
    return clockwise;
}


}
