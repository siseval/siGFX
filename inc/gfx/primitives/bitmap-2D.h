// #pragma once
//
// #include "gfx/core/primitive-2D.h"
// #include "gfx/core/types/color4.h"
// #include "gfx/core/types/bitmap.h"
// #include "gfx/math/box2.h"
// #include "gfx/math/matrix.h"
//
// namespace gfx
// {
//
// class Bitmap2D : public Primitive2D
// {
//
// public:
//
//     std::vector<Vec2i> rasterize(const Matrix3x3d &transform) const override;
//
//     Box2d get_geometry_size() const override;
//
//     bool point_collides(const Vec2d point, const Matrix3x3d &transform) const override;
//
//     inline void load_bitmap(Bitmap bitmap)
//     {
//         resolution = bitmap.resolution;
//         pixels = bitmap.pixels;
//         set_obb_dirty();
//     }
//
//     inline Color4 get_pixel(const Vec2i pixel) const 
//     { 
//         if (pixel.x < 0 || pixel.x >= resolution.x || pixel.y < 0 || pixel.y >= resolution.y) 
//         {
//             return { 0, 0, 0, 0 }; 
//         }
//         return pixels[pixel.y * resolution.x + pixel.x]; 
//     }
//     inline Color4 get_pixel(const int x, const int y) const 
//     { 
//         return get_pixel({ x, y }); 
//     }
//
//     inline void set_pixel(const Vec2i pixel, const Color4 color) 
//     {
//         if (pixel.x < 0 || pixel.x >= resolution.x || pixel.y < 0 || pixel.y >= resolution.y) 
//         {
//             return; 
//         }
//         pixels[pixel.y * resolution.x + pixel.x] = color;
//         set_obb_dirty();
//     };
//
//     inline void set_resolution(const Vec2i new_resolution) 
//     { 
//         resolution = new_resolution; 
//         pixels.resize(resolution.x * resolution.y); 
//         set_obb_dirty();
//     }
//     inline void set_resolution(const int width, const int height) { set_resolution({ width, height }); }
//     inline Vec2d get_resolution() const { return resolution; }
//
// private:
//
//     Vec2i resolution;
//     std::vector<Color4> pixels;
// };
//
// };
