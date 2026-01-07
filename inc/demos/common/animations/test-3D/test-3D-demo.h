#pragma once

#include "demos/common/core/gfx-demo.h"
#include "demos/common/core/demo-utils.h"

#include <map>

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
    void handle_char(const int input) override;
    void report_key(const demos::KeyEvent event) override;
    void report_mouse(const demos::MouseEvent event) override;
    std::vector<std::string> debug_text() override
    {
        return {
        };
    }

private:

    void update_camera(const double dt);
    void set_camera_velocity(const gfx::Vec3d velocity);
    void camera_movement(const Key key, const double dt);
    void poll_held_keys(const double dt);

    std::shared_ptr<gfx::Primitive3D> cube;
    std::shared_ptr<gfx::Primitive3D> sphere;
    std::shared_ptr<gfx::Primitive3D> floor_item;

    gfx::Vec2d prev_mouse_pos { 0.0, 0.0 };

    gfx::Vec3d camera_velocity { 0.0, 0.0, 0.0 };
    double camera_acceleration = 500.0;
    double max_camera_speed = 10.0;

    double last_frame_ms = 0.0;

    std::map<Key, bool> key_states {
        { Key::W, false },
        { Key::A, false },
        { Key::S, false },
        { Key::D, false },
        { Key::SPACE, false },
        { Key::CTRL, false },
    };

};

}
