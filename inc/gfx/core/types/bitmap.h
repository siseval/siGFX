#pragma once

#include "gfx/core/types/color4.h"
#include "gfx/math/vec2.h"

#include <vector>

namespace gfx
{

class Bitmap
{

public:

    Bitmap(const Vec2i resolution);

    void resize(const Vec2i new_resolution);

    void set_pixel(const Vec2i pos, const Color4 color);

    Color4 get_pixel(const Vec2i pos) const;

    void fill(const Color4 color = Color4 { 0, 0, 0, 255 });
    void compress_colors(const std::vector<Color4>& palette);

    static Bitmap decode_bmp(const std::string& filename);

private:

    Vec2i resolution;
    std::vector<Color4> pixels;

};

}
