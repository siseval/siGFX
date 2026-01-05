#pragma once

#include "gfx/core/types/color4.h"
#include "gfx/math/vec2.h"

namespace gfx
{

class RenderSurface 
{

public:

    enum class BlendMode
    {
        NONE,
        ALPHA,
    };

    RenderSurface(const Vec2i resolution)
        : resolution(resolution),
        frame_buffer(std::make_unique<std::vector<int32_t>>(resolution.x * resolution.y, 0)),
        depth_buffer(std::make_unique<std::vector<double>>(resolution.x * resolution.y, std::numeric_limits<double>::infinity()))
    {}

    virtual int init() = 0;

 
    void write_pixel(const Vec2i pos, const Color4 color, const double depth = 0, const BlendMode blend_mode = BlendMode::NONE)
    {
        if (pos.x < 0 || pos.y < 0 || pos.x >= resolution.x || pos.y >= resolution.y)
        {
            return;
        }

        const int index = pos.y * resolution.x + pos.x;
        if (depth > depth_buffer->at(index))
        {
            return;
        }
        depth_buffer->at(index) = depth;

        switch (blend_mode)
        {
            case BlendMode::NONE:
            {
                frame_buffer->data()[index] = std::byteswap(color.to_i32());
                return;
            }
            case BlendMode::ALPHA:
            {
                Color4 dst = Color4::from_i32(std::byteswap(frame_buffer->data()[index]));

                const double a = color.a_double();
                const double ia = 1.0 - a;

                Color4 out {
                    static_cast<uint8_t>((color.r_double() * a + dst.r_double() * ia) * 255.0),
                    static_cast<uint8_t>((color.g_double() * a + dst.g_double() * ia) * 255.0),
                    static_cast<uint8_t>((color.b_double() * a + dst.b_double() * ia) * 255.0),
                    static_cast<uint8_t>((a + dst.a_double() * ia) * 255.0)
                };

                frame_buffer->data()[index] = std::byteswap(out.to_i32());
                return;
            }
        }
    }

    virtual void present() = 0;
    virtual void clear() const = 0;

    virtual void clear_frame_buffer() = 0;
    virtual void clear_palette() = 0;

    virtual void resize(const Vec2i new_resolution) = 0;

    inline void set_resolution(const Vec2i new_resolution) { resolution = new_resolution; }
    inline Vec2i get_resolution() const { return resolution; }
 
    inline virtual void set_clear_color(const Color4 color) { clear_color = color; }
    inline virtual Color4 get_clear_color() const { return clear_color; }

protected:

    Vec2i resolution;
    Color4 clear_color = Color4(0.2, 0.2, 0.2, 1.0);
    std::unique_ptr<std::vector<int32_t>> frame_buffer;
    std::unique_ptr<std::vector<double>> depth_buffer;
};

}
