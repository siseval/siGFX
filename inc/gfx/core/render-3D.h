#pragma once

#include "gfx/core/camera.h"
#include "gfx/core/primitive-3D.h"
#include "gfx/core/render-surface.h"
#include "gfx/core/scene-graph-3D.h"
#include "gfx/primitives/cuboid-3D.h"
#include "gfx/primitives/plane-3D.h"
#include "gfx/primitives/sphere-3D.h"

#include <memory>

namespace gfx
{
    class Render3D
    {

    public:

        explicit Render3D(std::shared_ptr<RenderSurface> surface);

        void draw_frame() const;
        void clear_frame() const;
        void present_frame() const;

        void add_item(const std::shared_ptr<Primitive3D> &item) const;
        void remove_item(const std::shared_ptr<Primitive3D> &item) const;
        bool contains_item(const std::shared_ptr<Primitive3D> &item) const;
        void clear_items() const;

        std::shared_ptr<SceneGraph3D> get_scene_graph() const;
        std::shared_ptr<RenderSurface> get_render_surface() const;

        void set_camera(const Camera &cam);
        void set_camera_position(Vec3d pos);
        void set_camera_rotation(Vec3d rot);
        void set_camera_rotation_degrees(Vec3d rot_deg);
        void set_camera_fov(double fov);
        void set_camera_fov_degrees(double fov_degrees);
        void set_camera_z_near(double z_near);
        void set_camera_z_far(double z_far);

        void set_light_direction(Vec3d dir);
        void set_ambient_light(double intensity);

        void set_render_surface(std::shared_ptr<RenderSurface> new_surface);

        int num_items() const;
        Vec2i get_resolution() const;

        const Camera &get_camera() const;
        double get_camera_aspect_ratio() const;
        Vec3d get_camera_position() const;
        Vec3d get_camera_rotation() const;
        double get_camera_fov() const;
        Vec3d get_camera_forward() const;

        Vec3d get_light_direction() const;
        double get_ambient_light() const;

        static std::shared_ptr<Cuboid3D> create_cuboid(
            Vec3d position,
            Vec3d size,
            Color4 color,
            Shader3D shader = DefaultShader3D()
        );

        static std::shared_ptr<Sphere3D> create_sphere(
            Vec3d position,
            double radius,
            Color4 color,
            int segments = 16,
            Shader3D shader = DefaultShader3D()
        );

        static std::shared_ptr<Plane3D> create_plane(
            Vec3d position,
            Vec2d size,
            Color4 color,
            Shader3D shader = DefaultShader3D()
        );

    private:

        Camera camera;
        Vec3d light_dir;
        double ambient_light;

        std::shared_ptr<SceneGraph3D> scene_graph;
        std::shared_ptr<RenderSurface> surface;
    };
}
