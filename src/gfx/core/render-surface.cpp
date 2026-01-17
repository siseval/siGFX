#include "gfx/core/render-surface.h"

#include <bit>

namespace gfx
{

    RenderSurface::RenderSurface(const Vec2i resolution)
        : resolution(resolution),
          frame_buffer(std::vector(resolution.x * resolution.y, 0)),
          depth_buffer(std::vector(resolution.x * resolution.y, std::numeric_limits<double>::infinity())) {}

    void RenderSurface::write_pixel(const Vec2i pos, const Color4 color, const double depth, const BlendMode blend_mode)
    {
        if (pos.x < 0 || pos.y < 0 || pos.x >= resolution.x || pos.y >= resolution.y)
        {
            return;
        }

        const int index = pos.y * resolution.x + pos.x;
        if (depth > depth_buffer.at(index))
        {
            return;
        }
        depth_buffer.at(index) = depth;

        switch (blend_mode)
        {
        case BlendMode::OPAQUE:
            {
                frame_buffer.data()[index] = std::byteswap(color.to_i32());
                return;
            }
        case BlendMode::ALPHA:
            {
                const Color4 dst = Color4::from_i32(std::byteswap(frame_buffer.data()[index]));

                const double a = color.a_double();
                const double ia = 1.0 - a;

                const Color4 out {
                    static_cast<uint8_t>((color.r_double() * a + dst.r_double() * ia) * 255.0),
                    static_cast<uint8_t>((color.g_double() * a + dst.g_double() * ia) * 255.0),
                    static_cast<uint8_t>((color.b_double() * a + dst.b_double() * ia) * 255.0),
                    static_cast<uint8_t>((a + dst.a_double() * ia) * 255.0)
                };

                frame_buffer.data()[index] = std::byteswap(out.to_i32());
            }
        }
    }

    void RenderSurface::set_resolution(const Vec2i new_resolution)
    {
        resolution = new_resolution;
    }

    Vec2i RenderSurface::get_resolution() const
    {
        return resolution;
    }

    void RenderSurface::set_clear_color(const Color4 color)
    {
        clear_color = color;
    }

    Color4 RenderSurface::get_clear_color() const
    {
        return clear_color;
    }

}
