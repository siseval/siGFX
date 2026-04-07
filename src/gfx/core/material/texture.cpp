#include "gfx/core/material/texture.h"

namespace gfx
{

Texture::Texture(const Vec2i resolution)
    : resolution(resolution),
      data(resolution.x * resolution.y, Color4(0, 0, 0, 0)) {}

Texture::Texture(const Vec2i resolution, const std::vector<Color4> &data)
    : resolution(resolution),
      data(data) {}


void Texture::set_pixel(const Vec2i pos, const Color4 &color)
{
    if (pos.x < 0 || pos.x >= resolution.x ||
        pos.y < 0 || pos.y >= resolution.y)
    {
        return;
    }
    data[pos.y * resolution.x + pos.x] = color;
}

Color4 Texture::sample(const Vec2d uv) const
{
    const int x = static_cast<int>(uv.x * resolution.x) % resolution.x;
    const int y = static_cast<int>(uv.y * resolution.y) % resolution.y;
    return data[y * resolution.x + x];
}

}
