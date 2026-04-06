#include "demos/common/animations/test-3D/test-3D-demo.h"

#include "demos/common/core/demo-utils.h"
#include "gfx/shaders/diffuse-frag-shader.h"

using namespace gfx;

namespace demos
{

class FogShader : public Shader3D::FragShader
{
    Color4 frag(const Shader3D::FragInput &input) const override
    {
        const double near { uniforms.near_plane };
        const double far { uniforms.far_plane };

        const double depth { std::clamp(input.depth, 0.0, 1.0) };
        const double depth_linear = near * far / (far - depth * (far - near));

        const double dist { std::clamp((depth_linear / far) * 16.0, 0.0, 1.0) };

        const Color4 color {
            dist,
            dist,
            dist,
            0.5
        };

        return color;
    }
};

class DepthShader : public Shader3D::FragShader
{
    Color4 frag(const Shader3D::FragInput &input) const override
    {
        const double near { uniforms.near_plane };
        const double far { uniforms.far_plane };

        const double depth { std::clamp(input.depth, 0.0, 1.0) };
        const double depth_linear = near * far / (far - depth * (far - near));

        const double dist { std::clamp((depth_linear / far) * 16.0, 0.0, 1.0) };

        const Color4 color {
            dist,
            dist,
            dist,
            1.0
        };

        return color;
    }
};


class RedShader : public Shader3D::FragShader
{
    Color4 frag(const Shader3D::FragInput &input) const override
    {
        return Color4(
            std::clamp(input.color.r_double() * 1.5, 0.0, 1.0),
            input.color.g_double(),
            input.color.b_double(),
            1.0
        );
    }
};

class BandingShader : public Shader3D::FragShader
{
    Color4 frag(const Shader3D::FragInput &input) const override
    {
        const double step { 0.1 };
        return Color4(
            std::floor(input.color.r_double() / step) * step,
            std::floor(input.color.g_double() / step) * step,
            std::floor(input.color.b_double() / step) * step,
            1.0
        );
    }
};

class DiagonalShader : public Shader3D::FragShader
{
    Color4 frag(const Shader3D::FragInput &input) const override
    {
        const double t { uniforms.t };
        const Vec3d uvw { input.uvw };

        const double intensity { (input.color.r_double() + input.color.g_double() + input.color.b_double()) / 3 };

        double diagonal { uvw.x + uvw.y + t };
        diagonal = std::fmod(diagonal, std::numbers::pi);

        const double r { std::sin(diagonal) };
        const double g { 1 - std::sin(diagonal) };
        const double b = 0.5;

        return Color4(
            std::clamp(r, 0.0, 1.0),
            std::clamp(g, 0.0, 1.0),
            std::clamp(b, 0.0, 1.0),
            intensity
        );
    }
};

void Test3DDemo::init()
{
    renderer->set_clear_color(Color4(0.0, 0.0, 0.0));
    Camera &camera { renderer->get_camera() };

    camera.set_position(0.0, 5.0, 0.0);
    camera.set_rotation_degrees(0.0, 0.0, 0.0);
    camera.set_fov_degrees(75.0);
    camera.set_z_near(0.1);
    camera.set_z_far(1000.0);

    renderer->set_light_direction(-1.0, 1.0, -1.0);
    renderer->set_ambient_light(0.5);

    const Shader3D diffuse_shader(std::make_shared<DefaultVertShader>(), std::make_shared<DiffuseFragShader>());
    const Shader3D simple_shader(std::make_shared<DefaultVertShader>(), std::make_shared<DefaultFragShader>());
    const Shader3D shader2(std::make_shared<DefaultVertShader>(), std::make_shared<FogShader>());
    const Shader3D shader3(std::make_shared<DefaultVertShader>(), std::make_shared<BandingShader>());

    const Shader3D default_shader { diffuse_shader };

    sphere = renderer->create_sphere(Vec3d::zero(), 1.0, Color4(0.8, 0.4, 0.4), 16, default_shader);
    renderer->add_primitive(sphere);

    plane = renderer->create_plane(0.0, -15.0, 0.0, 20.0, 20.0, Color4(0.4, 0.8, 0.4), default_shader);
    renderer->add_primitive(plane);

    cube = renderer->create_cuboid(Vec3d::zero(), Vec3d { 2.0, 2.0, 2.0 }, Color4(0.4, 0.4, 0.8), default_shader);
    renderer->add_primitive(cube);

    cone = renderer->create_cone(Vec3d::zero(), 1.0, 2.0, Color4(0.8, 0.8, 0.4), 16, default_shader);

    crosshair = renderer->create_ellipse(
        renderer->get_resolution() / 2,
        { 1, 1 },
        Color4(1.0, 1.0, 1.0, 0.75),
        1.0
    );

    // renderer->get_render_3D()->add_fullscreen_shader(std::make_shared<DepthShader>());
    // renderer->get_render_3D()->add_fullscreen_shader(std::make_shared<DiagonalShader>());
    // renderer->get_render_3D()->add_fullscreen_shader(std::make_shared<BandingShader>());

    crosshair->set_anchor(0.5, 0.5);
    crosshair->set_filled(true);
    renderer->add_primitive(crosshair);

    constexpr double min_range = 128.0;
    constexpr double max_range = 256.0;
    constexpr int num_boxes = 0;
    constexpr int num_spheres = 10000;
    constexpr int num_segments = 3;

    const auto rand_pos = [](const double min, const double max) {
        return Vec3d::from_angles(random_double(0.0, 360.0), random_double(0.0, 180.0)).normalize() *
            random_double(min, max);
    };

    for (int i = 0; i < num_boxes; i++)
    {
        auto box = renderer->create_cuboid(
            rand_pos(min_range, max_range),
            Vec3d {
                random_double(1.0, 3.0),
                random_double(1.0, 3.0),
                random_double(1.0, 3.0)
            },
            Color4(0.8, 0.8, 0.6),
            default_shader
        );
        scene_items.push_back(box);
        renderer->add_primitive(box);
    }

    for (int i = 0; i < num_spheres; i++)
    {
        auto sphere = renderer->create_sphere(
            rand_pos(min_range, max_range),
            random_double(0.2, 0.6),
            Color4::white(),
            num_segments,
            default_shader
        );
        scene_items.push_back(sphere);
        renderer->add_primitive(sphere);
    }
}

void Test3DDemo::render_frame(const double dt)
{
    const double t_sec { time_sec() };

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
    Camera &camera = renderer->get_camera();
    camera.set_position(camera.get_position() + camera_velocity * dt);
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
    Vec3d forward { renderer->get_camera().get_forward() };
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
            const Vec2d delta { event.position - prev_mouse_pos };
            Camera &camera = renderer->get_camera();
            camera.set_rotation(
                {
                    camera.get_rotation().x + delta.y,
                    camera.get_rotation().y + delta.x,
                    camera.get_rotation().z
                }
            );
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
