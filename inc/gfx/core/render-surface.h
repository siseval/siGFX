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

    RenderSurface(const Vec2i resolution);

    virtual int init() = 0;

    void write_pixel(const Vec2i pos, const Color4 color, const double depth = 0, const BlendMode blend_mode = BlendMode::NONE);

    virtual void present() = 0;
    virtual void clear() const = 0;

    virtual void clear_frame_buffer() = 0;
    virtual void clear_palette() = 0;

    virtual void resize(const Vec2i new_resolution) = 0;

    void set_resolution(const Vec2i new_resolution);
    Vec2i get_resolution() const;

    virtual void set_clear_color(const Color4 color);
    virtual Color4 get_clear_color() const;

protected:

    Vec2i resolution;
    Color4 clear_color = Color4(0.2, 0.2, 0.2, 1.0);
    std::unique_ptr<std::vector<int32_t>> frame_buffer;
    std::unique_ptr<std::vector<double>> depth_buffer;
};

}
