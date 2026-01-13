#include "gfx/primitives/polyline-2D.h"

#include "gfx/geometry/rasterize.h"
#include "gfx/geometry/transform-2D.h"
#include "gfx/geometry/triangulate.h"
#include "gfx/geometry/types/polygon.h"
#include "gfx/geometry/types/barycentric-triangle.h"


namespace gfx
{

std::vector<Vec2i> Polyline2D::rasterize(const Matrix3x3d &transform) const
{
    std::vector<Vec2i> pixels;

    if (points.size() < 2)
    {
        return pixels;
    }

    for (int i = 0; i < points.size() - 1; ++i)
    {
        if (segments_visible.size() > i && !segments_visible[i])
        {
            continue;
        }
        rasterize_edge(points[i], points[i + 1], transform, pixels);
    }

    if (do_close)
    {
        rasterize_edge(points.back(), points.front(), transform, pixels);
    }

    if (do_rounded_corners)
    {
        rasterize_rounded_corners(transform, pixels);
    }

    if (do_fill)
    {
        std::vector<Vec2d> transformed_points { Transform2D::transform_points(points, transform) };

        Polygon::Component polygon(transformed_points, clockwise);
        std::vector<BarycentricTriangle> triangles { Triangulate::triangulate_polygon(polygon) };

        for (const auto& triangle : triangles)
        {
            Rasterize::rasterize_filled_triangle(triangle, pixels);
        }
    }

    return pixels;
}


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

void Polyline2D::rasterize_rounded_corner(const Vec2d pos, const double angle0, const double angle1, const Matrix3x3d &transform, std::vector<Vec2i> &pixels) const
{
    std::vector<Vec2d> vertices;

    for (int i = 0; i <= CORNER_SEGMENTS; ++i)
    {
        double progress { static_cast<double>(i) / static_cast<double>(CORNER_SEGMENTS) };
        double theta { angle0 + (angle1 - angle0) * progress };

        Vec2d vertex {
            pos.x + (line_thickness / 2.0) * std::cos(theta),
            pos.y + (line_thickness / 2.0) * std::sin(theta)
        };

        vertices.push_back(Transform2D::transform_point(vertex, transform));
    }
    Vec2d transformed_pos = Transform2D::transform_point(pos, transform);

    for (int i = 0; i < vertices.size() - 1; ++i)
    {
        Rasterize::rasterize_filled_triangle({ transformed_pos, vertices[i], vertices[i + 1] }, pixels);
    }
}

void Polyline2D::rasterize_rounded_corners(const Matrix3x3d &transform, std::vector<Vec2i> &pixels) const
{
    for (int i = 0; i < points.size(); ++i)
    {
        Vec2d p0 { points[(i - 1 + points.size()) % points.size()] };
        Vec2d p1 { points[i] };
        Vec2d p2 { points[(i + 1) % points.size()] };

        Vec2d normal0 { (p0 - p1).normal().normalize() };
        Vec2d normal1 { (p1 - p2).normal().normalize() };

        Vec2d between { ((p1 - p0) + (p1 - p2)).normalize() };

        double angle0 { std::atan2(normal0.y, normal0.x) };
        double angle1 { std::atan2(normal1.y, normal1.x) };

        double angle_diff { angle1 - angle0 };
        if (angle_diff <= 0) 
        {
            angle_diff += 2 * std::numbers::pi;
        }

        double angle_overlap = 0.1;
        double pos_overlap = 0.2;

        rasterize_rounded_corner(p1 - between * pos_overlap, angle0 - angle_overlap, angle0 + angle_diff + angle_overlap, transform, pixels);
    }
}

void Polyline2D::rasterize_edge(const Vec2d start, const Vec2d end, const Matrix3x3d &transform, std::vector<Vec2i> &pixels) const
{
    double line_extent { line_thickness / 2.0 };
    Vec2d normal { (end - start).normal().normalize() };

    Vec2d offset { normal * line_extent };

    Vec2d v0 { start + offset };
    Vec2d v1 { start - offset };
    Vec2d v2 { end + offset };
    Vec2d v3 { end - offset };

    v0 = Transform2D::transform_point(v0, transform);
    v1 = Transform2D::transform_point(v1, transform);
    v2 = Transform2D::transform_point(v2, transform);
    v3 = Transform2D::transform_point(v3, transform);

    Rasterize::rasterize_filled_triangle({ v0, v1, v2 }, pixels);
    Rasterize::rasterize_filled_triangle({ v1, v3, v2 }, pixels);
}






}
