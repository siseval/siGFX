#pragma once

#include "gfx/core/primitive-2D.h"
#include "gfx/math/box2.h"
#include "gfx/math/vec2.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Ellipse2D : public Primitive2D
{

public:

    std::vector<Vec2i> rasterize(const Matrix3x3d &transform) const override;

    Box2d get_geometry_size() const override;
    Box2d get_axis_aligned_bounding_box(const Matrix3x3d &transform) const override;

    bool point_collides(const Vec2d point, const Matrix3x3d &transform) const override;

    inline Vec2d get_radius() const { return radius; }
    inline void set_radius(const Vec2d r) { radius = r; set_obb_dirty(); }
    inline void set_radius(const double rx, const double ry) { radius = Vec2d { rx, ry }; set_obb_dirty(); }

    inline double get_line_thickness() const { return line_thickness; }
    inline void set_line_thickness(const double t) { line_thickness = t; set_obb_dirty(); }

    inline bool get_filled() const { return filled; }
    inline void set_filled(const bool f) { filled = f; }
    
private:

    Vec2d radius;
    double line_thickness = 1.0;
    bool filled = false;
    
    double MIN_MULTITHREAD_PIXELS { 10000 * 10000 };
};

};
