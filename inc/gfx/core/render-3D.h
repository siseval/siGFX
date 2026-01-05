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

    Vec3d get_camera_rotation() const
    {
        return camera.get_rotation();
    }

private:

    Camera camera;
    std::shared_ptr<gfx::SceneGraph3D> scene_graph;
    std::shared_ptr<gfx::RenderSurface> surface;

};

}

