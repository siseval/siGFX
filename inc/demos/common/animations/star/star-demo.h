#pragma once

#include <gfx/core/render-2D.h>

#include "demos/common/core/gfx-demo.h"

namespace demos
{

class StarDemo : public demos::GfxDemo
{

public:

    StarDemo(const std::shared_ptr<gfx::Render2D> renderer)
        : GfxDemo(renderer) {}

    void init() override;
    void render_frame(const double dt) override;
    void end() override;
    void handle_char(const int input) override;
    void report_mouse(const demos::MouseEvent event) override {}

private:

    int num_polylines = 32;
    std::vector<std::shared_ptr<gfx::Polyline2D>> polylines;
    std::vector<gfx::Color4> colors;
    int num_colors = 64;

    double speed = 2.0;
    double time = 0.0;
    bool paused = false;

};

}
