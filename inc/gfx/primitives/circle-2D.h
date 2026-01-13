#pragma once

#include "gfx/core/primitive-2D.h"
#include "gfx/math/box2.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Circle2D : public Primitive2D
{

public:

    Box2d get_geometry_size() const override;
    Box2d get_axis_aligned_bounding_box(const Matrix3x3d &transform) const override;

    bool point_collides(const Vec2d point, const Matrix3x3d &transform) const override;

    inline double get_radius() const { return radius; }
    inline void set_radius(const double r) { radius = r; set_obb_dirty(); }

    inline double get_line_thickness() const { return line_thickness; }
    inline void set_line_thickness(const double t) { line_thickness = t; set_obb_dirty(); }

    inline bool get_filled() const { return filled; }
    inline void set_filled(const bool f) { filled = f; }

    std::vector<Vec2i> rasterize(const Matrix3x3d &transform) const override;
    
private:

    double radius;
    bool filled = false;
    double line_thickness = 1.0;

};

};
