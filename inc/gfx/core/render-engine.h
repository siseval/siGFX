#pragma once

#include "gfx/core/primitive-2D.h"
#include "gfx/core/primitive-3D.h"
#include "gfx/core/render-2D.h"
#include "gfx/core/render-3D.h"
#include "gfx/core/render-surface.h"

namespace gfx
{
    class RenderEngine
    {

    public:

        explicit RenderEngine(std::shared_ptr<RenderSurface> surface);

        void render_frame() const;
        void present_frame() const;
        void clear_frame() const;

        void add_primitive(std::shared_ptr<Primitive2D> primitive) const;
        void add_primitive(std::shared_ptr<Primitive3D> primitive) const;
        void remove_primitive(std::shared_ptr<Primitive2D> primitive) const;
        void remove_primitive(std::shared_ptr<Primitive3D> primitive) const;
        bool contains_primitive(std::shared_ptr<Primitive2D> primitive) const;
        bool contains_primitive(std::shared_ptr<Primitive3D> primitive) const;
        void clear_scene() const;
        void clear_3D_scene() const;
        void clear_2D_scene() const;

        int num_primitives() const;
        int num_2D_primitives() const;
        int num_3D_primitives() const;

        void set_resolution(Vec2i new_resolution) const;
        void set_resolution(int width, int height) const;
        Vec2i get_resolution() const;

        void set_camera_position(Vec3d position) const;
        void set_camera_position(double x, double y, double z) const;
        void set_camera_rotation(Vec3d rotation_rad) const;
        void set_camera_rotation(double x_rad, double y_rad, double z_rad) const;
        void set_camera_rotation_degrees(Vec3d rotation_degrees) const;
        void set_camera_rotation_degrees(double x_deg, double y_deg, double z_deg) const;
        void set_camera_fov(double fov) const;
        void set_camera_fov_degrees(double fov_degrees) const;
        void set_camera_z_near(double z_near) const;
        void set_camera_z_far(double z_far) const;

        void set_light_direction(Vec3d direction) const;
        void set_light_direction(double x, double y, double z) const;
        void set_ambient_light(double intensity) const;

        Vec3d get_camera_position() const;
        Vec3d get_camera_rotation() const;
        double get_camera_fov() const;
        Vec3d get_camera_forward() const;

        Vec3d get_light_direction() const;
        double get_ambient_light() const;

        void set_font_directory(const std::filesystem::path &path) const;
        void load_font_directory(const std::filesystem::path &path = "") const;

        std::filesystem::path get_font_directory() const;
        std::shared_ptr<FontTTF> get_font(const std::string &name) const;
        bool is_font_loaded(const std::string &name) const;

        void set_render_surface(std::shared_ptr<RenderSurface> new_surface);
        std::shared_ptr<RenderSurface> get_render_surface() const;

        std::shared_ptr<Render2D> get_render_2D() const;
        std::shared_ptr<Render3D> get_render_3D() const;

        void set_clear_color(Color4 color) const;

        Color4 get_clear_color() const;

        std::shared_ptr<Circle2D> create_circle(
            Vec2d position,
            double radius,
            Color4 color,
            double line_thickness = 1.0
        ) const;

        std::shared_ptr<Circle2D> create_circle(
            double x,
            double y,
            double radius,
            Color4 color,
            double line_thickness = 1.0
        ) const;

        std::shared_ptr<Ellipse2D> create_ellipse(
            Vec2d position,
            Vec2d radius,
            Color4 color,
            double line_thickness = 1.0
        ) const;

        std::shared_ptr<Ellipse2D> create_ellipse(
            double x,
            double y,
            double radius_x,
            double radius_y,
            Color4 color,
            double line_thickness = 1.0
        ) const;

        std::shared_ptr<Polyline2D> create_polyline(
            Vec2d position,
            const std::vector<Vec2d> &points,
            Color4 color,
            double line_thickness = 1.0
        ) const;

        std::shared_ptr<Polyline2D> create_polyline(
            double x,
            double y,
            const std::vector<Vec2d> &points,
            Color4 color,
            double line_thickness = 1.0
        ) const;

        std::shared_ptr<Polygon2D> create_polygon(Vec2d position, const std::vector<Vec2d> &points, Color4 color) const;
        std::shared_ptr<Polygon2D> create_polygon(
            double x,
            double y,
            const std::vector<Vec2d> &points,
            Color4 color
        ) const;

        std::shared_ptr<Text2D> create_text(
            Vec2d position,
            const std::string &text,
            std::shared_ptr<FontTTF> font,
            double font_size,
            Color4 color
        ) const;

        std::shared_ptr<Text2D> create_text(
            double x,
            double y,
            const std::string &text,
            std::shared_ptr<FontTTF> font,
            double font_size,
            Color4 color
        ) const;

        std::shared_ptr<Cuboid3D> create_cuboid(
            Vec3d position,
            Vec3d size,
            Color4 color,
            Shader3D shader = DefaultShader3D()
        ) const;

        std::shared_ptr<Cuboid3D> create_cuboid(
            double x,
            double y,
            double z,
            double size_x,
            double size_y,
            double size_z,
            Color4 color,
            Shader3D shader = DefaultShader3D()
        ) const;

        std::shared_ptr<Plane3D> create_plane(
            Vec3d position,
            Vec2d size,
            Color4 color,
            Shader3D shader = DefaultShader3D()
        ) const;

        std::shared_ptr<Plane3D> create_plane(
            double x,
            double y,
            double z,
            double size_x,
            double size_y,
            Color4 color,
            Shader3D shader = DefaultShader3D()
        ) const;

        std::shared_ptr<Sphere3D> create_sphere(
            Vec3d position,
            double radius,
            Color4 color,
            int segments = 16,
            Shader3D shader = DefaultShader3D()
        ) const;

        std::shared_ptr<Sphere3D> create_sphere(
            double x,
            double y,
            double z,
            double radius,
            Color4 color,
            int segments = 16,
            Shader3D shader = DefaultShader3D()
        ) const;

    private:

        std::shared_ptr<RenderSurface> surface;

        std::shared_ptr<Render2D> render2D;
        std::shared_ptr<Render3D> render3D;
    };
}
