#include <locale.h>

#include "gfx/surfaces/curses/curses-render-surface.h"

namespace gfx
{

int CursesRenderSurface::init()
{
    setlocale(LC_ALL, "");

    initscr();
    timeout(0);
    cbreak();
    noecho();
    curs_set(0);

    clear_color = { 0, 0, 0, 0 };

    return 0;
}


void CursesRenderSurface::present()
{
    const Vec2i frame_buffer_dimensions { resolution / 2 };
    for (int y = 0; y < frame_buffer_dimensions.y; y++)
    {
        for (int x = 0; x < frame_buffer_dimensions.x; x++)
        {
            const int frame_buffer_index { y * resolution.x + x };
            if (frame_buffer_index < 0 || frame_buffer_index >= frame_buffer->size())
            {
                continue;
            }
            const int64_t pixel_value { frame_buffer->at(frame_buffer_index) };
            if ((pixel_value & 0x00000000000000FF) == 0)
            {
                continue;
            }

            std::string_view pixel {
                pixel_tree[(pixel_value & 0b1000) >> 3]
                [(pixel_value & 0b0100) >> 2]
                [(pixel_value & 0b0010) >> 1]
                [(pixel_value & 0b0001)]
            };

            const Color4 color { Color4::from_i32(pixel_value >> 32) };
            if (color.a == 0)
            {
                continue;
            }
            set_color(color);
            mvaddstr(y, x, pixel.data());
        }
    }
}

void CursesRenderSurface::clear() const
{
    erase();
}

void CursesRenderSurface::clear_frame_buffer()
{
    for (int64_t &pixel : *frame_buffer)
    {
        pixel = 0;
    }
}

void CursesRenderSurface::resize(const Vec2i new_resolution)
{
    resolution = new_resolution;
    frame_buffer->resize(resolution.x * resolution.y / 2, 0);
}

void CursesRenderSurface::clear_palette()
{
    palette->clear();
    color_index = 0;
}

void CursesRenderSurface::set_color(const Color4 color)
{
    const auto iterator { palette->find(color) };
    uint8_t index = 0;

    if (iterator != palette->end())
    {
        index = iterator->second;
    }
    else
    {
        index = add_color(color);
    }
    attron(COLOR_PAIR(index));
}

uint8_t CursesRenderSurface::add_color(const Color4 color)
{
    if (color_index + DEDICATED_CURSES_COLOR_START_INDEX >= 255)
    {
        color_index = 0;
    }
    uint8_t index { static_cast<uint8_t>(color_index + DEDICATED_CURSES_COLOR_START_INDEX) };
    palette->emplace(color, index);
    color_index += 1;
    init_color(index, color.r_double() * 1000, color.g_double() * 1000, color.b_double() * 1000);
    init_pair(index, index, -1);
    return index;
}

}
