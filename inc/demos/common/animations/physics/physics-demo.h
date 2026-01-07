#pragma once

#include <gfx/core/render-2D.h>

#include "demos/common/core/gfx-demo.h"

namespace demos
{

class PhysicsDemo : public demos::GfxDemo
{

public:

    PhysicsDemo(const std::shared_ptr<gfx::Render2D> renderer)
        : GfxDemo(renderer) {}

    void init() override;
    void render_frame(const double dt) override;
    void end() override;
    void handle_char(const int input) override;
    void report_mouse(const demos::MouseEvent event) override {}

private:

    int num_polylines = 32;
    std::vector<std::shared_ptr<gfx::Polyline2D>> polylines;

};

}
