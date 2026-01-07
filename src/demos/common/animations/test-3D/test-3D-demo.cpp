#include "demos/common/animations/test-3D/test-3D-demo.h"

#include "demos/common/core/demo-utils.h"
#include "gfx/core/camera.h"

using namespace gfx;

namespace demos
{

void Test3DDemo::init()
{
    Camera camera;
    camera.set_position({ 0.0, 0.0, -5.0 });
    camera.set_rotation({ 0.0, 0.0, 0.0 });
    camera.set_z_near(0.1);
    camera.set_z_far(1000.0);
    render3D->set_camera(camera);
    render3D->set_light_direction({ -1.0, 1.0, -1.0 });
    render3D->set_ambient_light(0.5);

    sphere = std::make_shared<Sphere3D>();
    sphere->set_radius(1.0);
    render3D->add_item(sphere);
    
    floor_item = std::make_shared<Plane3D>();
    floor_item->set_size(20.0, 20.0);
    floor_item->set_position(0.0, -8.0, 0.0);

    render3D->add_item(floor_item);

    cube = std::make_shared<Cuboid3D>();
    cube->set_size(2.0, 2.0, 2.0);


    render3D->add_item(cube);
}

void Test3DDemo::render_frame(const double dt)
{
    double t_sec { time_sec() };

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

    render2D->clear_frame();
    render3D->draw_frame();
    render2D->draw_frame();
    render2D->present_frame();

    last_frame_ms = time_ms();
}

void Test3DDemo::update_camera(const double dt)
{
    camera_velocity = camera_velocity * std::pow(0.8, dt * 60.0);
    camera_velocity = camera_velocity.limit(max_camera_speed);
    render3D->set_camera_position(render3D->get_camera().get_position() + camera_velocity * dt);
}

void Test3DDemo::handle_char(const int input)
{
}

void Test3DDemo::poll_held_keys(const double dt)
{
    for (const auto& [key, held] : key_states)
    {
        if (held)
        {
            camera_movement(key, dt);
        }
    }
}

void Test3DDemo::camera_movement(const Key input, const double dt)
{
    Vec3d cam_pos { render3D->get_camera().get_position() };
    Vec3d forward { render3D->get_camera().get_forward_vector() };
    forward.y = 0.0;

    switch (input)
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
                camera_velocity += Vec3d::cross(forward, Vec3d { 0, 1, 0 }).normalize() * camera_acceleration * dt;
                break;
            }
        case Key::D:
            {
                camera_velocity -= Vec3d::cross(forward, Vec3d { 0, 1, 0 }).normalize() * camera_acceleration * dt;
                break;
            }
        case Key::SPACE:
            {
                camera_velocity += Vec3d { 0, 1, 0 } * camera_acceleration * dt;
                break;
            }
        case Key::CTRL:
            {
                camera_velocity -= Vec3d { 0, 1, 0 } * camera_acceleration * dt;
                break;
            }
        default:
            {
                break;
            }
    }
}

void Test3DDemo::report_key(const demos::KeyEvent event)
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
}

void Test3DDemo::report_mouse(const demos::MouseEvent event)
{
    switch (event.type)
    {
        case MouseEventType::MOVE:
            {
                Vec2d delta { event.position - prev_mouse_pos };
                render3D->set_camera_rotation({
                    render3D->get_camera().get_rotation().x + delta.y,
                    render3D->get_camera().get_rotation().y + delta.x,
                    render3D->get_camera().get_rotation().z
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
    render2D->clear_items();
    render3D->clear_items();
}

}
