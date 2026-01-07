#pragma once

#include "gfx/core/scene-graph-3D.h"
#include "gfx/core/render-surface.h"
#include "gfx/core/camera.h"

#include <memory>

namespace gfx
{

class Render3D 
{

public:

    Render3D(std::shared_ptr<RenderSurface> surface) : 
        surface(surface), 
        scene_graph(std::make_shared<SceneGraph3D>())
    {
    }

    void draw_frame();
    void clear_frame()
    {
        surface->clear_frame_buffer();
        surface->clear();
    }

    void present_frame()
    {
        surface->present();
    }

    std::shared_ptr<SceneGraph3D> get_scene_graph() const { return scene_graph; };
    std::shared_ptr<RenderSurface> get_render_surface() const { return surface; };

    void add_item(const std::shared_ptr<Primitive3D>& item)
    {
        scene_graph->add_item(item);
    }

    void remove_item(const std::shared_ptr<Primitive3D>& item)
    {
        scene_graph->remove_item(item);
    }

    void clear_items()
    {
        scene_graph->clear();
    }

    Vec2i get_resolution() const 
    { 
        return surface->get_resolution();
    };

    double get_aspect_ratio() const
    {
        return static_cast<double>(get_resolution().x) / static_cast<double>(get_resolution().y);
    }

    void set_camera(const Camera& cam)
    {
        camera = cam;
    }

    const Camera& get_camera() const
    {
        return camera;
    }

    void set_camera_position(const Vec3d pos)
    {
        camera.set_position(pos);
    }

    Vec3d get_camera_position() const
    {
        return camera.get_position();
    }

    void set_camera_rotation(const Vec3d rot)
    {
        camera.set_rotation(rot);
    }

    void set_camera_rotation_degrees(const Vec3d rot_deg)
    {
        camera.set_rotation_degrees(rot_deg);
    }

    Vec3d get_camera_rotation() const
    {
        return camera.get_rotation();
    }

    void set_camera_fov(const double fov)
    {
        camera.set_fov(fov);
    }

    double get_camera_fov() const
    {
        return camera.get_fov();
    }

    Vec3d get_camera_forward() const
    {
        return camera.get_forward_vector();
    }

    void set_light_direction(const Vec3d dir)
    {
        light_dir = dir.normalize();
    }

    Vec3d get_light_direction() const
    {
        return light_dir;
    }

    void set_ambient_light(const double intensity)
    {
        ambient_light = intensity;
    }

    double get_ambient_light() const
    {
        return ambient_light;
    }

    void set_render_surface(const std::shared_ptr<RenderSurface> new_surface)
    {
        surface = new_surface;
    }

private:

    Camera camera;
    Vec3d light_dir { 0.0, 0.0, -1.0 };
    double ambient_light { 0.2 };

    std::shared_ptr<gfx::SceneGraph3D> scene_graph;
    std::shared_ptr<gfx::RenderSurface> surface;

};

}

