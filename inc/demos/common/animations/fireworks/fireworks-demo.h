#pragma once

#include <gfx/core/render-2D.h>

#include "demos/common/core/gfx-demo.h"
#include "demos/common/animations/fireworks/firework.h"

namespace demos
{

class FireworksDemo : public demos::GfxDemo
{

public:

    FireworksDemo(const std::shared_ptr<gfx::Render2D> renderer)
    : GfxDemo(renderer) {}

    void init() override;
    void render_frame(const double dt) override;
    void end() override;
    void handle_char(const int input) override;
    void report_mouse(const demos::MouseEvent event) override {}

    virtual std::vector<std::string> debug_text() override
    {
        return { 
            "dt: " + std::to_string(last_frame_us / 1000),
            "num fireworks: " + std::to_string(fireworks.size()),
            "velocity: " + std::to_string(fireworks.empty() ? 0.0 : fireworks.back().velocity.y)
        };
    }

private:

    void spawn_firework();

    Firework::FireworkOptions options;

    double firework_speed = 10.0;
    double angle_variation = 8.0;

    int max_fireworks = 10;
    double last_spawn_time_ms = 0.0;
    double spawn_interval_ms = 500.0;
    gfx::Vec2d spawn_margins;
    std::vector<Firework> fireworks;

    std::vector<std::vector<gfx::Color4>> color_combinations {
        { { 240, 40, 40 }, { 255, 90, 90 } },
        { { 240, 140, 0 }, { 255, 210, 0 } },
        { { 255, 255, 0 }, { 220, 220, 0 } },
        // { { 255, 180, 0 }, { 100, 180, 255 } },
        // { { 255, 255, 0 }, { 255, 0, 255 } },
        // { { 255, 25, 0 }, { 255, 180, 0 } },
        // { { 0, 255, 255 }, { 0, 255, 0 } },
    };

};

}
