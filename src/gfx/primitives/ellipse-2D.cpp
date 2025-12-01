#include "gfx/primitives/ellipse-2D.h"
#include "gfx/geometry/transform.h"

namespace gfx
{

Box2d Ellipse2D::get_geometry_size() const
{
    return Box2d { Vec2d::zero(), radius * 2 };
}

Box2d Ellipse2D::get_axis_aligned_bounding_box(const Matrix3x3d &transform) const
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
    std::vector<Vec2d> transformed_corners { Transform::transform_points(corners, transform) };

    Box2d bounds { transformed_corners[0], transformed_corners[0] };
    bounds.expand(transformed_corners);

    return bounds;
}


bool Ellipse2D::point_collides(const Vec2d point, const Matrix3x3d &transform) const
{
    Matrix3x3d inverse_transform { Transform::invert_affine(transform) };
    Vec2d local_point { Transform::transform_point(point, inverse_transform) - radius };

    return 
    (local_point.x * local_point.x) / (radius.x * radius.x) + 
    (local_point.y * local_point.y) / (radius.y * radius.y) <= 1.0;
}



// void Ellipse2D::rasterize_polygon_ring(std::shared_ptr<RenderSurface> surface, const Matrix3x3d &transform) const
// {
//     std::vector<Vec2d> outer_ring;
//     std::vector<Vec2d> inner_ring;
//     double line_extent = line_thickness / 2.0;
//
//     for (int i = 0; i < SEGMENTS; ++i)
//     {
//         double progress = static_cast<double>(i) / static_cast<double>(SEGMENTS);
//         double theta = 2.0 * std::numbers::pi * progress;
//
//         double outer_x = (radius.x + line_extent) * std::cos(theta) + radius.x;
//         double outer_y = (radius.y + line_extent) * std::sin(theta) + radius.y;
//         outer_ring.push_back(Vec2d { outer_x, outer_y });
//
//         double inner_x = (radius.x - line_extent) * std::cos(theta) + radius.x;
//         double inner_y = (radius.y - line_extent) * std::sin(theta) + radius.y;
//         inner_ring.push_back(Vec2d { inner_x, inner_y });
//     }
//
//     std::vector<Vec2d> transformed_outer_ring = transform_points(outer_ring, transform);
//     std::vector<Vec2d> transformed_inner_ring = transform_points(inner_ring, transform);
//
//     for (int i = 0; i < SEGMENTS; ++i)
//     {
//         Vec2d v0 = transformed_outer_ring[i];
//         Vec2d v1 = transformed_outer_ring[(i + 1) % SEGMENTS];
//         Vec2d v2 = transformed_inner_ring[i];
//         Vec2d v3 = transformed_inner_ring[(i + 1) % SEGMENTS];
//
//         rasterize_filled_triangle(surface, Triangle { v0, v1, v2 }, get_color());
//         rasterize_filled_triangle(surface, Triangle { v1, v3, v2 }, get_color());
//     }
// }


}
