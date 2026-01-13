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

    Vec2d get_radius() const;
    void set_radius(const Vec2d r);
    void set_radius(const double rx, const double ry);

    double get_line_thickness() const;
    void set_line_thickness(const double t);

    bool get_filled() const;
    void set_filled(const bool f);

private:

    Vec2d radius;
    double line_thickness = 1.0;
    bool filled = false;
    
    double MIN_MULTITHREAD_PIXELS { 10000 * 10000 };
};

};
