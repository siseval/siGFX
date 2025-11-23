#ifndef POLYLINE_2D_H
#define POLYLINE_2D_H

#include <gfx/core/primitive-2D.h>
#include <gfx/math/box2.h>
#include <gfx/math/vec2.h>
#include <gfx/math/matrix.h>
#include <gfx/geometry/triangle.h>
#include <gfx/utils/transform.h>
#include <gfx/geometry/triangulate.h>
#include <gfx/geometry/rasterize.h>
#include <gfx/geometry/types/polygon.h>

namespace gfx::primitives
{

class Polyline2D : public gfx::core::PrimitiveTemplate<Polyline2D>
{

public:

    gfx::math::Box2d get_geometry_size() const override;
    gfx::math::Box2d get_axis_aligned_bounding_box(const gfx::math::Matrix3x3d &transform) const override;

    bool point_collides(const gfx::math::Vec2d point, const gfx::math::Matrix3x3d &transform) const override;

    bool cache_clockwise();

    inline void add_point(const gfx::math::Vec2d point) { points.push_back(point); cache_clockwise(); set_obb_dirty(); }
    inline void add_point(const double x, const double y) { points.push_back(gfx::math::Vec2d { x, y }); cache_clockwise(); set_obb_dirty(); }
    inline void add_points(const std::vector<gfx::math::Vec2d> &new_points) { points.insert(points.end(), new_points.begin(), new_points.end()); cache_clockwise(); set_obb_dirty(); }

    inline void set_point(const size_t index, const gfx::math::Vec2d point) 
    { 
        if (index < points.size()) 
        { 
            points[index] = point; 
            cache_clockwise();
            set_obb_dirty();
        } 
    }
    inline void set_point(const size_t index, const double x, const double y) 
    { 
        if (index < points.size()) 
        { 
            points[index] = gfx::math::Vec2d { x, y }; 
            cache_clockwise();
            set_obb_dirty();
        } 
    }
    inline void set_points(const std::vector<gfx::math::Vec2d> &new_points) { points = new_points; cache_clockwise(); set_obb_dirty(); }
    inline void clear_points() { points.clear(); }

    inline void set_segment_visible(const size_t index, const bool visible) 
    { 
        if (segments_visible.size() < points.size()) 
        { 
            segments_visible.resize(points.size(), true); 
        }
        if (index < points.size()) 
        { 
            segments_visible[index] = visible; 
        } 
    }
    inline bool get_segment_visible(const size_t index) const 
    { 
        if (index < points.size()) 
        { 
            return segments_visible[index]; 
        } 
        return false; 
    }

    inline const std::vector<gfx::math::Vec2d> get_points() const { return points; }
    inline gfx::math::Vec2d get_point(const size_t index) const 
    { 
        if (index < points.size()) 
        { 
            return points[index]; 
        } 
        return gfx::math::Vec2d::zero(); 
    }

    inline size_t get_num_points() const { return points.size(); }

    inline void set_close(const bool close) { do_close = close; }
    inline bool get_close() const { return do_close; }

    inline void set_rounded_corners(const bool rounded) { do_rounded_corners = rounded; set_obb_dirty(); }
    inline bool get_rounded_corners() const { return do_rounded_corners; }

    inline void set_line_thickness(const double t) { line_thickness = t; set_obb_dirty(); }
    inline double get_line_thickness() const { return line_thickness; }

    inline void set_fill(const bool f) { do_fill = f; }
    inline bool get_fill() const { return do_fill; }

    template<typename EmitPixel>
    void rasterize(const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        if (points.size() < 2)
        {
            return;
        }

        for (int i = 0; i < points.size() - 1; ++i)
        {
            if (segments_visible.size() > i && !segments_visible[i])
            {
                continue;
            }
            rasterize_edge(points[i], points[i + 1], transform, emit_pixel);
        }

        if (do_close)
        {
            rasterize_edge(points.back(), points.front(), transform, emit_pixel);
        }

        if (do_rounded_corners)
        {
            rasterize_rounded_corners(transform, emit_pixel);
        }

        if (do_fill)
        {
            std::vector<gfx::math::Vec2d> transformed_points { utils::transform_points(points, transform) };

            gfx::geometry::types::Component polygon { gfx::geometry::types::Component(transformed_points, clockwise) };
            std::vector<gfx::geometry::Triangle> triangles { geometry::triangulate_polygon(polygon) };

            for (const auto& triangle : triangles)
            {
                geometry::rasterize_filled_triangle(triangle, color, emit_pixel);
            }
        }
    }

    template<typename EmitPixel>
    void rasterize_rounded_corner(const gfx::math::Vec2d pos, const double angle0, const double angle1, const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        std::vector<gfx::math::Vec2d> vertices;

        for (int i = 0; i <= CORNER_SEGMENTS; ++i)
        {
            double progress { static_cast<double>(i) / static_cast<double>(CORNER_SEGMENTS) };
            double theta { angle0 + (angle1 - angle0) * progress };

            gfx::math::Vec2d vertex {
                pos.x + (line_thickness / 2.0) * std::cos(theta),
                pos.y + (line_thickness / 2.0) * std::sin(theta)
            };

            vertices.push_back(utils::transform_point(vertex, transform));
        }
        gfx::math::Vec2d transformed_pos = utils::transform_point(pos, transform);

        for (int i = 0; i < vertices.size() - 1; ++i)
        {
            geometry::rasterize_filled_triangle({ transformed_pos, vertices[i], vertices[i + 1] }, color, emit_pixel);
        }
    }

    template<typename EmitPixel>
    void rasterize_rounded_corners(const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        for (int i = 0; i < points.size(); ++i)
        {
            gfx::math::Vec2d p0 { points[(i - 1 + points.size()) % points.size()] };
            gfx::math::Vec2d p1 { points[i] };
            gfx::math::Vec2d p2 { points[(i + 1) % points.size()] };

            gfx::math::Vec2d normal0 { (p0 - p1).normal().normalize() };
            gfx::math::Vec2d normal1 { (p1 - p2).normal().normalize() };

            gfx::math::Vec2d between { ((p1 - p0) + (p1 - p2)).normalize() };

            double angle0 { std::atan2(normal0.y, normal0.x) };
            double angle1 { std::atan2(normal1.y, normal1.x) };

            double angle_diff { angle1 - angle0 };
            if (angle_diff <= 0) 
            {
                angle_diff += 2 * std::numbers::pi;
            }

            double angle_overlap = 0.1;
            double pos_overlap = 0.2;

            rasterize_rounded_corner(p1 - between * pos_overlap, angle0 - angle_overlap, angle0 + angle_diff + angle_overlap, transform, emit_pixel);
        }
    }

    template<typename EmitPixel>
    void rasterize_edge(const gfx::math::Vec2d start, const gfx::math::Vec2d end, const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        double line_extent { line_thickness / 2.0 };
        gfx::math::Vec2d normal { (end - start).normal().normalize() };

        gfx::math::Vec2d offset { normal * line_extent };

        gfx::math::Vec2d v0 { start + offset };
        gfx::math::Vec2d v1 { start - offset };
        gfx::math::Vec2d v2 { end + offset };
        gfx::math::Vec2d v3 { end - offset };

        v0 = utils::transform_point(v0, transform);
        v1 = utils::transform_point(v1, transform);
        v2 = utils::transform_point(v2, transform);
        v3 = utils::transform_point(v3, transform);

        geometry::rasterize_filled_triangle({ v0, v1, v2 }, color, emit_pixel);
        geometry::rasterize_filled_triangle({ v1, v3, v2 }, color, emit_pixel);
    }

    std::vector<gfx::math::Vec2d> points;
    std::vector<bool> segments_visible;
    bool do_close = false;
    bool do_fill = false;
    double line_thickness = 1.0;
    bool do_rounded_corners = false;
    bool clockwise = false;
    static constexpr int CORNER_SEGMENTS = 8;
};

};

#endif // POLYLINE_2D_H
