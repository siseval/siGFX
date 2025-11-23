#ifndef CIRCLE_2D_H
#define CIRCLE_2D_H

#include <gfx/core/render-surface.h>
#include <gfx/core/primitive-2D.h>
#include <gfx/math/box2.h>
#include <gfx/math/matrix.h>
#include <gfx/utils/transform.h>

namespace gfx::primitives
{

class Circle2D : public gfx::core::PrimitiveTemplate<Circle2D>
{

public:

    gfx::math::Box2d get_geometry_size() const override;
    gfx::math::Box2d get_axis_aligned_bounding_box(const gfx::math::Matrix3x3d &transform) const override;

    bool point_collides(const gfx::math::Vec2d point, const gfx::math::Matrix3x3d &transform) const override;

    inline double get_radius() const { return radius; }
    inline void set_radius(const double r) { radius = r; set_obb_dirty(); }

    inline double get_line_thickness() const { return line_thickness; }
    inline void set_line_thickness(const double t) { line_thickness = t; set_obb_dirty(); }

    inline bool get_filled() const { return filled; }
    inline void set_filled(const bool f) { filled = f; }

    template<typename EmitPixel>
    void rasterize(const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        if (radius <= 0)
        {
            return;
        }

        double line_extent { line_thickness / 2.0 };
        gfx::math::Box2d AABB { get_axis_aligned_bounding_box(transform) };
        gfx::math::Matrix3x3d inverse_transform { utils::invert_affine(transform) };
        for (int y = AABB.min.y; y <= AABB.max.y; y++)
        {
            for (int x = AABB.min.x; x <= AABB.max.x; x++)
            {
                gfx::math::Vec2d pos { 
                    utils::transform_point(
                        gfx::math::Vec2d { 
                            static_cast<double>(x) , 
                            static_cast<double>(y) 
                        }, inverse_transform) - gfx::math::Vec2d(radius) 
                };

                double r_outer { radius + line_extent };
                double r_inner { radius - line_extent };

                double distance { std::sqrt(pos.x * pos.x + pos.y * pos.y) };

                if (distance <= r_outer && (get_filled() || distance >= r_inner))
                {
                    emit_pixel(gfx::core::types::Pixel { { x, y }, get_color() });
                    continue;
                }
            }
        }
    }

private:

    double radius;
    bool filled = false;
    double line_thickness = 1.0;

};

};

#endif // CIRCLE_2D_H
