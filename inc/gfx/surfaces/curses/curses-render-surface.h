#pragma once

#include <string_view>
#include <ncurses.h>

#include "gfx/core/render-surface.h"

namespace gfx
{

class CursesRenderSurface : public RenderSurface
{

public:

    CursesRenderSurface(const Vec2i resolution) 
        : RenderSurface(resolution), 
        palette(std::make_unique<std::unordered_map<Color4, uint8_t, std::hash<Color4>>>()), 
        frame_buffer(std::make_unique<std::vector<int64_t>>(resolution.x * resolution.y / 2, 0))
        {};

    int init() override;

    void present() override;
    void clear() const override;

    void clear_frame_buffer() override;

    void write_pixel(const Vec2i pos, const Color4 color, const int depth = 0);

    void resize(const Vec2i new_resolution) override;

    void clear_palette() override;

private:

    void render_multithreaded();
    void set_color(const Color4 color);
    uint8_t add_color(const Color4 color);

    std::unique_ptr<std::vector<int64_t>> frame_buffer;

    std::unique_ptr<std::unordered_map<Color4, uint8_t, std::hash<Color4>>> palette;
    int color_index = 0;

    static constexpr uint8_t DEDICATED_CURSES_COLOR_START_INDEX = 127;

    static constexpr std::string_view pixel_tree[2][2][2][2] {
        { // TOP LEFT 0
            { //TOP RIGHT 0
                { // BOT LEFT 0
                    { // BOT RIGHT 0
                            " \0"
                    },
                    { // BOT RIGHT 1
                            "⠠\0"
                    }
                },
                { // BOT LEFT 1
                    { // BOT RIGHT 0
                            "⠄\0"
                    },
                    { // BOT RIGHT 1
                            "⠤\0"
                    }
                }
            },
            { // TOP RIGHT 1
                { // BOT LEFT 0
                    { // BOT RIGHT 0
                            "⠈\0"
                    },
                    { // BOT RIGHT 1
                            "⠨\0"
                    }
                },
                { // BOT LEFT 1
                    { // BOT RIGHT 0
                            "⠌\0"
                    },
                    { // BOT RIGHT 1
                            "⠬\0"
                    },
                }
            }
        },
        { // TOP LEFT 1
            { // TOP RIGHT 0
                { // BOT LEFT 0
                    { // BOT RIGHT 0
                            "⠁\0"
                    },
                    { // BOT RIGHT 1
                            "⠡\0"
                    }
                },
                { // BOT LEFT 1
                    { // BOT RIGHT 0
                            "⠅\0"
                    },
                    { // BOT RIGHT 1
                            "⠥\0"
                    }
                }
            },
            { // TOP RIGHT 1
                { // BOT LEFT 0
                    { // BOT RIGHT 0
                            "⠉\0"
                    },
                    { // BOT RIGHT 1
                            "⠩\0"
                    }
                },
                { // BOT LEFT 1
                    { // BOT RIGHT 0
                            "⠍\0"
                    },
                    { // BOT RIGHT 1
                            "⠭\0"
                    }
                }
            }
        }
    };

};

}
