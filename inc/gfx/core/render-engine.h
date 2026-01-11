#pragma once

#include "gfx/core/render-surface.h"
#include "gfx/core/render-2D.h"
#include "gfx/core/render-3D.h"
#include "gfx/core/primitive-3D.h"
#include "gfx/core/primitive-2D.h"

#include <memory>

namespace gfx
{

class RenderEngine
{

public:

    RenderEngine(std::shared_ptr<RenderSurface> surface)
        : surface(surface)
    {
        render2D = std::make_shared<Render2D>(surface);
        render3D = std::make_shared<Render3D>(surface);
    }

    void render_frame();
    void present_frame();
    void clear_frame();

    void add_primitive(const std::shared_ptr<Primitive2D> primitive);
    void add_primitive(const std::shared_ptr<Primitive3D> primitive);
    void clear_scene();
    void clear_3D_scene();
    void clear_2D_scene();

    int num_primitives() const;
    int num_2D_primitives() const;
    int num_3D_primitives() const;

    void set_resolution(const Vec2i new_resolution);
    void set_resolution(const int width, const int height);
    Vec2i get_resolution() const;

    void set_camera_position(const Vec3d position);
    void set_camera_position(const double x, const double y, const double z);
    void set_camera_rotation(const Vec3d rotation_rad);
    void set_camera_rotation(const double x_rad, const double y_rad, const double z_rad);
    void set_camera_rotation_degrees(const Vec3d rotation_degrees);
    void set_camera_rotation_degrees(const double x_deg, const double y_deg, const double z_deg);
    void set_camera_fov(const double fov);
    void set_camera_fov_degrees(const double fov_degrees);
    void set_camera_z_near(const double z_near);
    void set_camera_z_far(const double z_far);

    void set_light_direction(const Vec3d direction);
    void set_light_direction(const double x, const double y, const double z);
    void set_ambient_light(const double intensity);

    Vec3d get_camera_position() const;
    Vec3d get_camera_rotation() const;
    double get_camera_fov() const;
    Vec3d get_camera_forward() const;

    Vec3d get_light_direction() const;
    double get_ambient_light() const;

    void set_font_directory(const std::filesystem::path &path);
    void load_font_directory(const std::filesystem::path &path = "");

    std::filesystem::path get_font_directory() const;
    std::shared_ptr<FontTTF> get_font(const std::string &name) const;
    bool is_font_loaded(const std::string &name) const;

    void debug_viewer_enable(const bool enable);
    void debug_viewer_show_aabb(const bool show);
    void debug_viewer_show_obb(const bool show);
    void debug_viewer_show_anchor(const bool show);
    void debug_viewer_set_font(const std::shared_ptr<FontTTF> font);

    bool is_debug_viewer_enabled() const;
    bool is_debug_viewer_showing_aabb() const;
    bool is_debug_viewer_showing_obb() const;
    bool is_debug_viewer_showing_anchor() const;

    void set_render_surface(const std::shared_ptr<RenderSurface> new_surface);
    std::shared_ptr<RenderSurface> get_render_surface() const;

    std::shared_ptr<Render2D> get_render_2D() const;
    std::shared_ptr<Render3D> get_render_3D() const;

    void set_clear_color(const Color4 color);

    Color4 get_clear_color() const;

    std::shared_ptr<Circle2D> create_circle(const Vec2d position, const double radius, const Color4 color, const double line_thickness = 1.0) const;
    std::shared_ptr<Circle2D> create_circle(const double x, const double y, const double radius, const Color4 color, const double line_thickness = 1.0) const;

    std::shared_ptr<Ellipse2D> create_ellipse(const Vec2d position, const Vec2d radius, const Color4 color, const double line_thickness = 1.0) const;
    std::shared_ptr<Ellipse2D> create_ellipse(const double x, const double y, const double radius_x, const double radius_y, const Color4 color, const double line_thickness = 1.0) const;

    std::shared_ptr<Polyline2D> create_polyline(const Vec2d position, const std::vector<Vec2d> &points, const Color4 color, const double line_thickness = 1.0) const;
    std::shared_ptr<Polyline2D> create_polyline(const double x, const double y, const std::vector<Vec2d> &points, const Color4 color, const double line_thickness = 1.0) const;

    std::shared_ptr<Polygon2D> create_polygon(const Vec2d position, const std::vector<Vec2d> &points, const Color4 fill_color) const;
    std::shared_ptr<Polygon2D> create_polygon(const double x, const double y, const std::vector<Vec2d> &points, const Color4 fill_color) const;

    std::shared_ptr<Bitmap2D> create_bitmap(const Vec2d position, const Bitmap &bm) const;
    std::shared_ptr<Bitmap2D> create_bitmap(const double x, const double y, const Bitmap &bm) const;

    std::shared_ptr<Bitmap2D> create_bitmap(const Vec2d position, const Vec2i resolution) const;
    std::shared_ptr<Bitmap2D> create_bitmap(const double x, const double y, const Vec2i resolution) const;

    std::shared_ptr<Text2D> create_text(const Vec2d position, const std::string &text, const std::shared_ptr<FontTTF> font, const double font_size, const Color4 color) const;
    std::shared_ptr<Text2D> create_text(const double x, const double y, const std::string &text, const std::shared_ptr<FontTTF> font, const double font_size, const Color4 color) const;

    std::shared_ptr<Cuboid3D> create_cuboid(const Vec3d position, const Vec3d size, const Color4 color) const;
    std::shared_ptr<Cuboid3D> create_cuboid(const double x, const double y, const double z, const double size_x, const double size_y, const double size_z, const Color4 color) const;

    std::shared_ptr<Plane3D> create_plane(const Vec3d position, const Vec2d size, const Color4 color) const;
    std::shared_ptr<Plane3D> create_plane(const double x, const double y, const double z, const double size_x, const double size_y, const Color4 color) const;

    std::shared_ptr<Sphere3D> create_sphere(const Vec3d position, const double radius, const Color4 color, const int segments = 16) const;
    std::shared_ptr<Sphere3D> create_sphere(const double x, const double y, const double z, const double radius, const Color4 color, const int segments = 16) const;

private:

    std::shared_ptr<RenderSurface> surface;

    std::shared_ptr<Render2D> render2D;
    std::shared_ptr<Render3D> render3D;

};

}
