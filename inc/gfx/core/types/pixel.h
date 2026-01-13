#pragma once

#include "gfx/core/types/color4.h"
#include "gfx/math/vec2.h"

namespace gfx
{

struct Pixel
{
    Vec2i position;
    double depth;
    Color4 color;
};

}
