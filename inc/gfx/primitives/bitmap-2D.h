#ifndef BITMAP_2D_H
#define BITMAP_2D_H

#include <gfx/core/render-surface.h>
#include <gfx/core/primitive-2D.h>
#include <gfx/core/types/color4.h>
#include <gfx/core/types/bitmap.h>
#include <gfx/math/box2.h>
#include <gfx/math/matrix.h>
#include <gfx/utils/transform.h>

namespace gfx::primitives
{

class Bitmap2D : public gfx::core::PrimitiveTemplate<Bitmap2D>
{

public:

    gfx::math::Box2d get_geometry_size() const override;

    bool point_collides(const gfx::math::Vec2d point, const gfx::math::Matrix3x3d &transform) const override;

    inline void load_bitmap(gfx::core::types::Bitmap bitmap)
    {
        resolution = bitmap.resolution;
        pixels = bitmap.pixels;
        set_obb_dirty();
    }

    inline gfx::core::types::Color4 get_pixel(const gfx::math::Vec2i pixel) const 
    { 
        if (pixel.x < 0 || pixel.x >= resolution.x || pixel.y < 0 || pixel.y >= resolution.y) 
        {
            return { 0, 0, 0, 0 }; 
        }
        return pixels[pixel.y * resolution.x + pixel.x]; 
    }
    inline gfx::core::types::Color4 get_pixel(const int x, const int y) const 
    { 
        return get_pixel({ x, y }); 
    }

    inline void set_pixel(const gfx::math::Vec2i pixel, const gfx::core::types::Color4 color) 
    {
        if (pixel.x < 0 || pixel.x >= resolution.x || pixel.y < 0 || pixel.y >= resolution.y) 
        {
            return; 
        }
        pixels[pixel.y * resolution.x + pixel.x] = color;
        set_obb_dirty();
    };

    inline void set_resolution(const gfx::math::Vec2i new_resolution) 
    { 
        resolution = new_resolution; 
        pixels.resize(resolution.x * resolution.y); 
        set_obb_dirty();
    }
    inline void set_resolution(const int width, const int height) { set_resolution({ width, height }); }
    inline gfx::math::Vec2d get_resolution() const { return resolution; }

    template<typename EmitPixel>
    void rasterize(const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        gfx::math::Box2d AABB { get_axis_aligned_bounding_box(transform) };
        gfx::math::Matrix3x3d inverse_transform { gfx::utils::invert_affine(transform) };

        for (int y = static_cast<int>(AABB.min.y); y < static_cast<int>(AABB.max.y); ++y)
        {
            for (int x = static_cast<int>(AABB.min.x); x < static_cast<int>(AABB.max.x); ++x)
            {
                gfx::math::Vec2d pos { static_cast<double>(x), static_cast<double>(y) };
                gfx::math::Vec2d local_pos = utils::transform_point(pos, inverse_transform);

                int img_x = static_cast<int>(local_pos.x);
                int img_y = static_cast<int>(local_pos.y);

                if (img_x >= 0 && img_x < resolution.x && img_y >= 0 && img_y < resolution.y)
                {
                    gfx::core::types::Color4 pixel { get_pixel({ img_x, img_y }) };
                    if (pixel.a > 0)
                    {
                        emit_pixel(gfx::core::types::Pixel { { x, y }, pixel });
                    }
                }
            }
        }
    }

private:

    gfx::math::Vec2i resolution;
    std::vector<gfx::core::types::Color4> pixels;
};

};

#endif // BITMAP_2D_H
