#pragma once

#include "demos/common/core/gfx-demo.h"

namespace demos
{

class Test3DDemo : public GfxDemo
{

public:

    Test3DDemo(const std::shared_ptr<gfx::Render2D> renderer,
                 const std::shared_ptr<gfx::Render3D> renderer3D)
        : GfxDemo(renderer, renderer3D) {}

    void init() override;
    void render_frame(const double dt) override;
    void end() override;
    void handle_input(const int input) override;
    void report_mouse(const demos::MouseEvent event) override;
    std::vector<std::string> debug_text() override
    {
        return {
        };
    }

private:

    std::shared_ptr<gfx::Primitive3D> item;
    gfx::Vec2d prev_mouse_pos { 0.0, 0.0 };

};

}
