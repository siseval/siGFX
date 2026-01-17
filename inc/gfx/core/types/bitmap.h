#pragma once

#include "gfx/core/types/color4.h"
#include "gfx/math/vec2.h"

#include <vector>

namespace gfx
{
    class Bitmap
    {

    public:

        explicit Bitmap(Vec2i resolution);

        void resize(Vec2i new_resolution);

        void set_pixel(Vec2i pos, Color4 color);

        Color4 get_pixel(Vec2i pos) const;

        void fill(Color4 color = Color4 { 0, 0, 0, 255 });
        void compress_colors(const std::vector<Color4> &palette);

        static Bitmap decode_bmp(const std::string &filename);

    private:

        Vec2i resolution;
        std::vector<Color4> pixels;
    };
}
