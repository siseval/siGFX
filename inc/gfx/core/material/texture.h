#pragma once

#include "gfx/core/types/color4.h"
#include "gfx/math/vec2.h"

namespace gfx
{

class Texture
{

public:

    Texture() = default;
    explicit Texture(const Vec2i resolution);
    Texture(const Vec2i resolution, const std::vector<Color4> &data);

    void set_pixel(const Vec2i pos, const Color4 &color);
    void set_pixel(const int x, const int y, const Color4 &color);
    void set_data(const std::vector<Color4> &data);
    Color4 sample(const Vec2d uv) const;

private:

    Vec2i resolution;
    std::vector<Color4> data;

};

}
