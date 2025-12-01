#ifndef RENDER_SURFACE_H
#define RENDER_SURFACE_H

#include <gfx/core/types/color4.h>
#include <gfx/math/vec2.h>

namespace gfx::core
{

class RenderSurface 
{

public:

    RenderSurface(const gfx::math::Vec2i resolution)
        : resolution(resolution),
        frame_buffer(std::make_unique<std::vector<int32_t>>(resolution.x * resolution.y, 0))
    {}

    virtual int init() = 0;

 
    void write_pixel(const gfx::math::Vec2i pos, const gfx::core::types::Color4 color, const int depth = 0)
    {
        if (pos.x < 0 || pos.y < 0 || pos.x >= resolution.x || pos.y >= resolution.y)
        {
            return;
        }

        const int index = pos.y * resolution.x + pos.x;

        frame_buffer->data()[index] = std::byteswap(color.to_i32());
    }

    virtual void present() = 0;
    virtual void clear() const = 0;

    virtual void clear_frame_buffer() = 0;
    virtual void clear_palette() = 0;

    virtual void resize(const gfx::math::Vec2i new_resolution) = 0;

    inline void set_resolution(const gfx::math::Vec2i new_resolution) { resolution = new_resolution; }
    inline gfx::math::Vec2i get_resolution() const { return resolution; }

    inline virtual void set_clear_color(const types::Color4 color) { clear_color = color; }
    inline virtual types::Color4 get_clear_color() const { return clear_color; }

protected:

    gfx::math::Vec2i resolution;
    gfx::core::types::Color4 clear_color = gfx::core::types::Color4(0.2, 0.2, 0.2, 1.0);
    std::unique_ptr<std::vector<int32_t>> frame_buffer;
};

}

#endif // RENDER_SURFACE_H
