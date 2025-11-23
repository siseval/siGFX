#include <gfx/primitives/circle-2D.h>
#include <gfx/utils/transform.h>

namespace gfx::primitives
{

using namespace gfx::core;
using namespace gfx::math;
using namespace gfx::core::types;


Box2d Circle2D::get_geometry_size() const
{
    return Box2d { Vec2d::zero(), { radius * 2 } };
}

Box2d Circle2D::get_axis_aligned_bounding_box(const Matrix3x3d &transform) const
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
    std::vector<Vec2d> transformed_corners { utils::transform_points(corners, transform) };

    Box2d bounds { transformed_corners[0], transformed_corners[0] };
    bounds.expand(transformed_corners);

    return bounds;
}

bool Circle2D::point_collides(const gfx::math::Vec2d point, const gfx::math::Matrix3x3d &transform) const
{
    Matrix3x3d inverse_transform { utils::invert_affine(transform) };
    Vec2d local_point { utils::transform_point(point, inverse_transform) - Vec2d(radius) };

    return local_point.x * local_point.x + local_point.y * local_point.y <= radius * radius;
}


}
