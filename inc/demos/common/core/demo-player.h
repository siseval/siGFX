#pragma once

#include <gfx/core/render-2D.h>
#include <gfx/core/render-3D.h>

#include "demos/common/core/gfx-demo.h"

namespace demos
{

class DemoPlayer
{

public:

    DemoPlayer() : render2D(std::shared_ptr<gfx::Render2D>()) {}

    void init();
    void run();

    void resize(const gfx::Vec2i new_resolution);
    bool screen_size_changed() { return get_screen_size() != render2D->get_render_surface()->get_resolution(); }

protected:

    void cycle_demo(const int direction);
    void handle_input(const int input);
    std::vector<std::string> get_info();

    virtual gfx::Vec2i get_screen_size() = 0;

    virtual int get_input() = 0;
    virtual void draw_info() = 0;

    std::shared_ptr<gfx::Render2D> render2D;
    std::shared_ptr<gfx::Render3D> render3D;
    std::vector<std::shared_ptr<GfxDemo>> demos;
    int current_demo = 0;

    bool show_info = true;
    bool show_debug = true;

    bool running = true;

};

}
