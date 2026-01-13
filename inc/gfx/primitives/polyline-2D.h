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

    bool cache_clockwise();

    void add_point(const Vec2d point);
    void add_point(const double x, const double y);
    void add_points(const std::vector<Vec2d> &new_points);

    void set_point(const size_t index, const Vec2d point);
    void set_point(const size_t index, const double x, const double y);
    void set_points(const std::vector<Vec2d> &new_points);
    void clear_points();

    void set_segment_visible(const size_t index, const bool visible);
    bool get_segment_visible(const size_t index) const;

    const std::vector<Vec2d> get_points() const;
    Vec2d get_point(const size_t index) const;

    size_t get_num_points() const;

    void set_close(const bool close);
    bool get_close() const;

    void set_rounded_corners(const bool rounded);
    bool get_rounded_corners() const;

    void set_line_thickness(const double t);
    double get_line_thickness() const;

    void set_fill(const bool f);
    bool get_fill() const;

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

}
