#include "gfx/primitives/circle-2D.h"

#include "gfx/geometry/transform-2D.h"

namespace gfx
{

std::vector<Vec2i> Circle2D::rasterize(const Matrix3x3d &transform) const
{
    std::vector<Vec2i> pixels;

    if (radius <= 0)
    {
        return pixels;
    }

    double line_extent { line_thickness / 2.0 };
    Box2d AABB { get_axis_aligned_bounding_box(transform) };
    Matrix3x3d inverse_transform { Transform2D::invert_affine(transform) };

    for (int y = AABB.min.y; y <= AABB.max.y; y++)
    {
        for (int x = AABB.min.x; x <= AABB.max.x; x++)
        {
            Vec2d pos { 
                Transform2D::transform_point(
                    Vec2d { 
                        static_cast<double>(x) , 
                        static_cast<double>(y) 
                    }, inverse_transform) - Vec2d(radius) 
            };

            double r_outer { radius + line_extent };
            double r_inner { radius - line_extent };

            double distance { std::sqrt(pos.x * pos.x + pos.y * pos.y) };

            if (distance <= r_outer && (get_filled() || distance >= r_inner))
            {
                pixels.push_back(Vec2i { x, y });
                continue;
            }
        }
    }

    return pixels;
}


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
    std::vector<Vec2d> transformed_corners { Transform2D::transform_points(corners, transform) };

    Box2d bounds { transformed_corners[0], transformed_corners[0] };
    bounds.expand(transformed_corners);

    return bounds;
}

double Circle2D::get_radius() const
{
    return radius;
}

void Circle2D::set_radius(const double r)
{
    radius = r;
    set_obb_dirty();
}

double Circle2D::get_line_thickness() const
{
    return line_thickness;
}

void Circle2D::set_line_thickness(const double t)
{
    line_thickness = t;
    set_obb_dirty();
}

bool Circle2D::get_filled() const
{
    return filled;
}

void Circle2D::set_filled(const bool f)
{
    filled = f;
}


}
