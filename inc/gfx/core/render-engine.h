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

    void set_camera_position(const Vec3d position);
    void set_camera_position(const double x, const double y, const double z);
    void set_camera_rotation(const Vec3d rotation_rad);
    void set_camera_rotation(const double x_rad, const double y_rad, const double z_rad);
    void set_camera_rotation_degrees(const Vec3d rotation_degrees);
    void set_camera_rotation_degrees(const double x_deg, const double y_deg, const double z_deg);
    void set_camera_fov(const double fov_degrees);
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
    void set_font_directory(const std::string &path);
    void load_fonts_from_directory(const std::filesystem::path &path = "");
    void load_fonts_from_directory(const std::string &path);

    std::filesystem::path get_font_directory() const;
    std::shared_ptr<FontTTF> get_font(const std::string &name) const;

    void debug_viewer_enable(const bool enable);
    void debug_viewer_show_aabb(const bool show);
    void debug_viewer_show_obb(const bool show);
    void debug_viewer_show_anchor(const bool show);
    void debug_viewer_set_font(const std::shared_ptr<FontTTF> font);

    void set_render_surface(const std::shared_ptr<RenderSurface> new_surface);

    void set_clear_color(const Color4 color);

private:

    std::shared_ptr<RenderSurface> surface;

    std::shared_ptr<Render2D> render2D;
    std::shared_ptr<Render3D> render3D;

};

}
