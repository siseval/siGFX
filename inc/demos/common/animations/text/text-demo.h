#pragma once

#include <gfx/core/render-2D.h>

#include "demos/common/core/gfx-demo.h"

namespace demos
{

class TextDemo : public demos::GfxDemo
{

public:

    TextDemo(const std::shared_ptr<gfx::Render2D> renderer)
        : GfxDemo(renderer) {}

    void init() override;
    void render_frame(const double dt) override;
    void end() override;
    void handle_input(const int input) override;
    void report_mouse(const demos::MouseEvent event) override {}

    std::vector<std::string> debug_text() override
    {
        return { 
            "num points: " + std::to_string(num_points),
            // "holes: " + std::to_string(polygon->get_hole_vertices().size())
        };
    }

    inline gfx::Color4 get_clear_color() const override 
    { 
        return gfx::Color4(0.0, 0.0, 0.0, 1.0); 
    };

private:

    bool is_clockwise(std::vector<gfx::Vec2d> vertices);

    std::shared_ptr<gfx::Text2D> text_item;
    int num_points = 0;

};

}
