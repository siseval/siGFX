#ifndef ELLIPSE_2D_H
#define ELLIPSE_2D_H

#include <thread>

#include <gfx/core/render-surface.h>
#include <gfx/core/primitive-2D.h>
#include <gfx/math/box2.h>
#include <gfx/math/vec2.h>
#include <gfx/math/matrix.h>
#include <gfx/utils/transform.h>

namespace gfx::primitives
{

class Ellipse2D : public gfx::core::PrimitiveTemplate<Ellipse2D>
{

public:

    gfx::math::Box2d get_geometry_size() const override;
    gfx::math::Box2d get_axis_aligned_bounding_box(const gfx::math::Matrix3x3d &transform) const override;

    bool point_collides(const gfx::math::Vec2d point, const gfx::math::Matrix3x3d &transform) const override;

    inline gfx::math::Vec2d get_radius() const { return radius; }
    inline void set_radius(const gfx::math::Vec2d r) { radius = r; set_obb_dirty(); }
    inline void set_radius(const double rx, const double ry) { radius = gfx::math::Vec2d { rx, ry }; set_obb_dirty(); }

    inline double get_line_thickness() const { return line_thickness; }
    inline void set_line_thickness(const double t) { line_thickness = t; set_obb_dirty(); }

    inline bool get_filled() const { return filled; }
    inline void set_filled(const bool f) { filled = f; }

    template<typename EmitPixel>
    void rasterize(const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        if (radius.x <= 0 || radius.y <= 0)
        {
            return;
        }

        double line_extent { line_thickness / 2.0 };
        gfx::math::Box2d AABB { get_axis_aligned_bounding_box(transform) };
        gfx::math::Matrix3x3d inverse_transform { utils::invert_affine(transform) };

        auto worker = [&](int start_y, int end_y) {
            for (int y = start_y; y <= end_y; y++)
            {
                for (int x = AABB.min.x; x <= AABB.max.x; x++)
                {
                    gfx::math::Vec2d pos { utils::transform_point(gfx::math::Vec2d { static_cast<double>(x) , static_cast<double>(y) }, inverse_transform) - radius };
                    gfx::math::Vec2d r_outer { radius + gfx::math::Vec2d(line_extent) };
                    gfx::math::Vec2d r_inner { radius - gfx::math::Vec2d(line_extent) };

                    double sdf_outer { (pos.x * pos.x) / (r_outer.x * r_outer.x) + (pos.y * pos.y) / (r_outer.y * r_outer.y) };
                    double sdf_inner { (pos.x * pos.x) / (r_inner.x * r_inner.x) + (pos.y * pos.y) / (r_inner.y * r_inner.y) };

                    if (sdf_outer <= 1.0 && (get_filled() || sdf_inner >= 1.0))
                    {
                        emit_pixel(gfx::core::types::Pixel { { x, y }, get_color() });
                        continue;
                    }
                }
            }
        };

        gfx::math::Vec2d size { AABB.size() };
        if (size.x * size.y < MIN_MULTITHREAD_PIXELS)
        {
            worker(static_cast<int>(AABB.min.y), static_cast<int>(AABB.max.y));
            return;
        }

        unsigned int num_threads { std::thread::hardware_concurrency() };
        std::vector<std::thread> threads;

        for (unsigned int i = 0; i < num_threads; ++i)
        {
            int start_y { static_cast<int>(AABB.min.y) + static_cast<int>(i * size.y / num_threads) };
            int end_y { static_cast<int>(AABB.min.y) + static_cast<int>((i + 1) * size.y / num_threads) - 1 };

            if (i == num_threads - 1)
            {
                end_y = static_cast<int>(AABB.max.y);
            }

            threads.emplace_back(worker, start_y, end_y);
        }

        for (auto &thread : threads)
        {
            thread.join();
        }
    }

private:

    gfx::math::Vec2d radius;
    double line_thickness = 1.0;
    bool filled = false;
    
    double MIN_MULTITHREAD_PIXELS { 100 * 100 };
};

};

#endif // ELLIPSE_2D_H
