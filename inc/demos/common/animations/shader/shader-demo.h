#ifndef SHADER_DEMO_H
#define SHADER_DEMO_H

#include <gfx/core/render-2D.h>
#include <demos/common/core/gfx-demo.h>
#include <gfx/math/vec3.h>

namespace demos::common::animations::shader
{

using namespace gfx::core::types;
using namespace gfx::math;
using namespace gfx::primitives;

struct Ripple
{
    Vec2d center;
    double start_time;
};

class WaterSurfaceShader : public gfx::core::Shader2D<WaterSurfaceShader>
{

public:

    Vec2d mouse_uv;
    std::vector<std::shared_ptr<Ripple>> ripples;
    double ripple_lifetime_sec = 5.0;
    gfx::core::types::Color4 base_color = gfx::core::types::Color4(0.0, 0.3, 0.5);

    mutable gfx::core::types::Color4 prev_color;

    gfx::core::types::Color4 frag(const gfx::core::ShaderInput2D &input) const;
};


class ShaderDemo : public demos::common::core::GfxDemo
{

public:

    ShaderDemo(const std::shared_ptr<gfx::core::Render2D> renderer)
        : GfxDemo(renderer) {}

    void init() override;
    void render_frame(const double dt) override;
    void end() override;
    void handle_input(const int input) override;
    void report_mouse(const demos::common::core::MouseEvent event) override;

    std::vector<std::string> debug_text() override
    {
        gfx::math::Vec2d mouse_uv { quad->get_uv(mouse_position) };
        WaterSurfaceShader water_shader = shader;
        return {
            "uv: (" + std::to_string(mouse_uv.x) + ", " + std::to_string(mouse_uv.y) + ")"
            "\nnum_ripples: " + std::to_string(water_shader.ripples.size())
            // "\nprev_color: " + std::to_string(water_shader.prev_color.r_double()) + ", " + std::to_string(water_shader->prev_color.g_double()) + ", " + std::to_string(water_shader->prev_color.b_double())
        };
    }

private:

    void update_primitives(const double dt);
    void wrap_position(gfx::math::Vec2d &position);
    void spawn_ripple(const gfx::math::Vec2d position);
    void update_ripples(const double dt);
    gfx::math::Vec2d get_random_position();

    WaterSurfaceShader shader;

    std::shared_ptr<gfx::primitives::Polygon2D> quad;

    gfx::math::Vec2d mouse_position;
    double spawn_timer;
    double spawn_interval_sec = 0.2;
    bool random_spawn = true;
    double random_spawn_interval_sec = 0.2;
    double random_spawn_timer = 0.0;

};

}

#endif // SHADER_DEMO_H
