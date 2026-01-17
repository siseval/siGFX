#pragma once

#include "demos/common/core/gfx-demo.h"
#include "gfx/primitives/cuboid-3D.h"
#include "gfx/primitives/plane-3D.h"
#include "gfx/primitives/sphere-3D.h"

#include <map>

namespace demos
{

    class Test3DDemo : public GfxDemo
    {

    public:

        Test3DDemo(const std::shared_ptr<gfx::RenderEngine> renderer)
            : GfxDemo(renderer) {}

        void init() override;
        void render_frame(double dt) override;
        void end() override;
        void handle_char(int input) override;
        void report_key(demos::KeyEvent event) override;
        void report_mouse(demos::MouseEvent event) override;

        std::vector<std::string> debug_text() override
        {
            return {
            };
        }

    private:

        void update_camera(double dt);
        void set_camera_velocity(gfx::Vec3d velocity);
        void camera_movement(Key key, double dt);
        void poll_held_keys(double dt);

        std::shared_ptr<gfx::Cuboid3D> cube;
        std::shared_ptr<gfx::Sphere3D> sphere;
        std::shared_ptr<gfx::Plane3D> floor_item;

        std::vector<std::shared_ptr<gfx::Primitive3D>> scene_items;

        std::shared_ptr<gfx::Ellipse2D> crosshair;

        gfx::Vec2d prev_mouse_pos { 0.0, 0.0 };

        gfx::Vec3d camera_velocity { 0.0, 0.0, 0.0 };
        double camera_acceleration = 500.0;
        double camera_damping = 0.85;
        double max_camera_speed = 30.0;

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
