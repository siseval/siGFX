#include "demos/common/animations/test-3D/test-3D-demo.h"

#include "demos/common/core/demo-utils.h"
#include "gfx/shaders/diffuse-frag-shader.h"

using namespace gfx;

namespace demos
{

    void Test3DDemo::init()
    {
        renderer->set_clear_color(Color4(0.0, 0.0, 0.0));

        renderer->set_camera_position(0.0, 5.0, 0.0);
        renderer->set_camera_rotation_degrees(0.0, 0.0, 0.0);
        renderer->set_camera_fov_degrees(75.0);
        renderer->set_camera_z_near(0.1);
        renderer->set_camera_z_far(1000.0);

        renderer->set_light_direction(-1.0, 1.0, -1.0);
        renderer->set_ambient_light(0.5);

        const Shader3D shader(std::make_shared<DefaultVertShader>(), std::make_shared<DiffuseFragShader>());

        sphere = renderer->create_sphere(Vec3d::zero(), 1.0, Color4(0.8, 0.4, 0.4), 16, shader);
        renderer->add_primitive(sphere);

        floor_item = renderer->create_plane(0.0, -15.0, 0.0, 20.0, 20.0, Color4(0.4, 0.8, 0.4), shader);
        renderer->add_primitive(floor_item);

        cube = renderer->create_cuboid(Vec3d::zero(), Vec3d{2.0, 2.0, 2.0}, Color4(0.4, 0.4, 0.8), shader);
        renderer->add_primitive(cube);

        crosshair = renderer->create_ellipse(renderer->get_resolution() / 2, {1, 1}, Color4(1.0, 1.0, 1.0, 0.75), 1.0);
        crosshair->set_anchor(0.5, 0.5);
        crosshair->set_filled(true);
        renderer->add_primitive(crosshair);

        constexpr double min_range = 256.0;
        constexpr double max_range = 512.0;
        constexpr int num_boxes = 0;
        constexpr int num_spheres = 800;
        constexpr int num_segments = 5;

        auto rand_pos = [](const double min, const double max) {
            return Vec3d::from_angles(random_double(0.0, 360.0), random_double(0.0, 180.0)).normalize() *
                random_double(min, max);
        };

        for (int i = 0; i < num_boxes; i++)
        {
            auto box = renderer->create_cuboid(
                rand_pos(min_range, max_range),
                Vec3d{
                    random_double(1.0, 3.0),
                    random_double(1.0, 3.0),
                    random_double(1.0, 3.0)
                },
                Color4(0.8, 0.8, 0.6),
                shader
            );
            scene_items.push_back(box);
            renderer->add_primitive(box);
        }

        for (int i = 0; i < num_spheres; i++)
        {
            auto sphere = renderer->create_sphere(
                rand_pos(min_range, max_range),
                random_double(1.0, 2.0),
                Color4::white(),
                num_segments,
                shader
            );
            scene_items.push_back(sphere);
            renderer->add_primitive(sphere);
        }
    }

    void Test3DDemo::render_frame(const double dt)
    {
        const double t_sec{time_sec()};

        for (const auto primitive : scene_items)
        {
            primitive->set_rotation_degrees(
                primitive->get_rotation_degrees().x + dt * 15.0,
                primitive->get_rotation_degrees().y + dt * 30.0,
                primitive->get_rotation_degrees().z + dt * 45.0
            );
        }

        cube->set_rotation_degrees(
            cube->get_rotation_degrees().x + dt * 30.0,
            cube->get_rotation_degrees().y + dt * 45.0,
            cube->get_rotation_degrees().z + dt * 60.0
        );

        cube->set_position(
            cube->get_position().x,
            std::sin(t_sec) * 2.0,
            cube->get_position().z
        );

        cube->set_scale(
            1.0,
            1.0,
            1.0
        );

        sphere->set_position(
            std::cos(t_sec) * 5.0,
            std::sin(t_sec) * 2.0,
            std::sin(t_sec) * 5.0
        );

        poll_held_keys(dt);
        update_camera(dt);

        renderer->clear_frame();
        renderer->render_frame();
        renderer->present_frame();
    }

    void Test3DDemo::update_camera(const double dt)
    {
        camera_velocity = camera_velocity * std::pow(0.85, dt * 60.0);
        camera_velocity = camera_velocity.limit(max_camera_speed);
        renderer->set_camera_position(renderer->get_camera_position() + camera_velocity * dt);
    }

    void Test3DDemo::handle_char(const int input) {}

    void Test3DDemo::poll_held_keys(const double dt)
    {
        for (const auto &[key, held] : key_states)
        {
            if (held)
            {
                camera_movement(key, dt);
            }
        }
    }

    void Test3DDemo::camera_movement(const Key key, const double dt)
    {
        Vec3d forward{renderer->get_camera_forward()};
        forward.y = 0.0;

        switch (key)
        {
        case Key::W:
            {
                camera_velocity += forward * camera_acceleration * dt;
                break;
            }
        case Key::S:
            {
                camera_velocity -= forward * camera_acceleration * dt;
                break;
            }
        case Key::A:
            {
                camera_velocity += Vec3d::cross(forward, Vec3d{0, 1, 0}).normalize() * camera_acceleration * dt;
                break;
            }
        case Key::D:
            {
                camera_velocity -= Vec3d::cross(forward, Vec3d{0, 1, 0}).normalize() * camera_acceleration * dt;
                break;
            }
        case Key::SPACE:
            {
                camera_velocity += Vec3d{0, 1, 0} * camera_acceleration * dt;
                break;
            }
        case Key::CTRL:
            {
                camera_velocity -= Vec3d{0, 1, 0} * camera_acceleration * dt;
                break;
            }
        default:
            {
                break;
            }
        }
    }

    void Test3DDemo::report_key(const KeyEvent event)
    {
        switch (event.type)
        {
        case KeyEventType::KEY_PRESS:
            {
                key_states[event.key] = true;
                break;
            }
        case KeyEventType::KEY_RELEASE:
            {
                key_states[event.key] = false;
                break;
            }
        case KeyEventType::KEY_REPEAT:
            {
                break;
            }
        }
        if (event.type == KeyEventType::KEY_PRESS)
        {
            switch (event.key)
            {
            case Key::E:
                {
                    crosshair->set_visible(!crosshair->is_visible());
                    break;
                }
            default:
                {
                    break;
                }
            }
        }
    }

    void Test3DDemo::report_mouse(const MouseEvent event)
    {
        switch (event.type)
        {
        case MouseEventType::MOVE:
            {
                const Vec2d delta{event.position - prev_mouse_pos};
                renderer->set_camera_rotation({
                    renderer->get_camera_rotation().x + delta.y,
                    renderer->get_camera_rotation().y + delta.x,
                    renderer->get_camera_rotation().z
                });
                prev_mouse_pos = event.position;
                break;
            }
        case MouseEventType::LEFT_DOWN:
            {
                break;
            }
        case MouseEventType::LEFT_UP:
            {
                break;
            }
        case MouseEventType::RIGHT_DOWN:
            {
                break;
            }
        case MouseEventType::RIGHT_UP:
            {
                break;
            }
        case MouseEventType::SCROLL_UP:
            {
                break;
            }
        case MouseEventType::SCROLL_DOWN:
            {
                break;
            }
        }
    }

    void Test3DDemo::end()
    {
        renderer->clear_scene();
    }

}
