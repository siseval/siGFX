#pragma once

#include "demos/common/core/gfx-demo.h"

#include <gfx/core/render-engine.h>
#include <gfx/debug/debug-viewer.h>

namespace demos
{

class DemoPlayer
{

public:

    DemoPlayer() : renderer(std::shared_ptr<gfx::RenderEngine>()) {}

    void init();
    void run();

    void resize(const gfx::Vec2i new_resolution) const;
    bool screen_size_changed() { return get_screen_size() != renderer->get_resolution(); }

protected:

    void cycle_demo(const int direction);
    void handle_input(const int input);
    std::vector<std::string> get_info() const;

    virtual gfx::Vec2i get_screen_size() = 0;

    virtual int get_input() = 0;
    virtual void draw_info() = 0;

    std::shared_ptr<gfx::RenderEngine> renderer;
    std::vector<std::shared_ptr<GfxDemo>> demos;
    int current_demo = 0;

    gfx::DebugViewer debug_viewer;

    bool show_info = true;
    bool show_debug = true;

    bool running = true;

};

}
