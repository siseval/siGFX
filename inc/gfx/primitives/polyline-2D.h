#pragma once

#include "gfx/core/primitive-2D.h"
#include "gfx/math/box2.h"
#include "gfx/math/vec2.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Polyline2D : public Primitive2D
{

public:

    std::vector<Vec2i> rasterize(const Matrix3x3d &transform) const override;

    Box2d get_geometry_size() const override;
    Box2d get_axis_aligned_bounding_box(const Matrix3x3d &transform) const override;

    bool point_collides(const Vec2d point, const Matrix3x3d &transform) const override;

    bool cache_clockwise();

    inline void add_point(const Vec2d point) { points.push_back(point); cache_clockwise(); set_obb_dirty(); }
    inline void add_point(const double x, const double y) { points.push_back(Vec2d { x, y }); cache_clockwise(); set_obb_dirty(); }
    inline void add_points(const std::vector<Vec2d> &new_points) { points.insert(points.end(), new_points.begin(), new_points.end()); cache_clockwise(); set_obb_dirty(); }

    inline void set_point(const size_t index, const Vec2d point) 
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
            points[index] = Vec2d { x, y }; 
            cache_clockwise();
            set_obb_dirty();
        } 
    }
    inline void set_points(const std::vector<Vec2d> &new_points) { points = new_points; cache_clockwise(); set_obb_dirty(); }
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

    inline const std::vector<Vec2d> get_points() const { return points; }
    inline Vec2d get_point(const size_t index) const 
    { 
        if (index < points.size()) 
        { 
            return points[index]; 
        } 
        return Vec2d::zero(); 
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

    void rasterize_rounded_corner(const Vec2d pos, const double angle0, const double angle1, const Matrix3x3d &transform, std::vector<Vec2i> &pixels) const;
    void rasterize_rounded_corners(const Matrix3x3d &transform, std::vector<Vec2i> &pixels) const;
    void rasterize_edge(const Vec2d start, const Vec2d end, const Matrix3x3d &transform, std::vector<Vec2i> &pixels) const;
    
    std::vector<Vec2d> points;
    std::vector<bool> segments_visible;
    bool do_close = false;
    bool do_fill = false;
    double line_thickness = 1.0;
    bool do_rounded_corners = false;
    bool clockwise = false;
    static constexpr int CORNER_SEGMENTS = 8;
};

};
