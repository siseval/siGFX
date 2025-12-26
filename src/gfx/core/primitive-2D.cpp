#include "gfx/core/primitive-2D.h"
#include "gfx/geometry/transform.h"

namespace gfx
{

Box2d Primitive2D::get_axis_aligned_bounding_box(const Matrix3x3d &transform) const
{
    Box2d extent { get_geometry_size() };
    Vec2d top_left { extent.min };
    Vec2d bot_right { extent.max };

    std::vector<Vec2d> corners {
        { top_left.x, top_left.y },
        { bot_right.x, top_left.y },
        { top_left.x, bot_right.y },
        { bot_right.x, bot_right.y },
    };
    std::vector<Vec2d> transformed_corners { Transform::transform_points(corners, transform) };

    Box2d bounds { transformed_corners[0], transformed_corners[0] };
    bounds.expand(transformed_corners);

    return bounds;
}

OBB2D Primitive2D::get_oriented_bounding_box(const Matrix3x3d &transform) const
{
    if (!obb_dirty)
    {
        return cached_obb;
    }

    Box2d extent { get_geometry_size() };

    OBB2D bounds {
        extent.min,
        { extent.max.x - extent.min.x, 0 },
        { 0, extent.max.y - extent.min.y }
    };
   
    bounds.origin = Transform::transform_point(bounds.origin, transform);
    bounds.side_x = Transform::transform_vector(bounds.side_x, transform);
    bounds.side_y = Transform::transform_vector(bounds.side_y, transform);

    cached_obb = bounds;
    obb_dirty = false;

    return bounds;
}

Vec2d Primitive2D::get_uv(const Vec2d point) const
{
    OBB2D obb { get_oriented_bounding_box(get_transform()) };
    return obb.get_uv(point);
}

Matrix3x3d Primitive2D::get_transform() const
{
    if (!transform_dirty)
    {
        return cached_transform;
    }

    Vec2d size { get_geometry_size().size() };
    Vec2d anchor_offset { get_anchor() * size };

    Matrix3x3d anchor_translation_matrix { Transform::translate(-anchor_offset) };
    Matrix3x3d scale_matrix { Transform::scale(scale) };
    Matrix3x3d rotation_matrix { Transform::rotate(rotation) };
    Matrix3x3d position_translation_matrix { Transform::translate(get_position()) };

<<<<<<< HEAD
    cached_transform = 
        position_translation_matrix * rotation_matrix * 
        scale_matrix * anchor_translation_matrix;

=======
    cached_transform = position_translation_matrix * rotation_matrix * scale_matrix * anchor_translation_matrix;
>>>>>>> dffdb0631f6dcb0f6cdaf39a229293a5db8bc9df
    transform_dirty = false;

    return cached_transform;
}

}


