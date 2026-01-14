#include "gfx/core/render-engine.h"

namespace gfx
{

RenderEngine::RenderEngine(std::shared_ptr<RenderSurface> surface) : surface(surface)
{
    render2D = std::make_shared<Render2D>(surface);
    render3D = std::make_shared<Render3D>(surface);
}

void RenderEngine::render_frame()
{
    render3D->draw_frame();
    render2D->draw_frame();
}

void RenderEngine::present_frame()
{
    surface->present();
}

void RenderEngine::clear_frame()
{
    surface->clear_frame_buffer();
}

void RenderEngine::add_primitive(const std::shared_ptr<Primitive2D> primitive)
{
    render2D->add_item(primitive);
}

void RenderEngine::add_primitive(const std::shared_ptr<Primitive3D> primitive)
{
    render3D->add_item(primitive);
}

void RenderEngine::remove_primitive(const std::shared_ptr<Primitive2D> primitive)
{
    render2D->remove_item(primitive);
}

void RenderEngine::remove_primitive(const std::shared_ptr<Primitive3D> primitive)
{
    render3D->remove_item(primitive);
}

bool RenderEngine::contains_primitive(const std::shared_ptr<Primitive2D> primitive) const
{
    return render2D->contains_item(primitive);
}

bool RenderEngine::contains_primitive(const std::shared_ptr<Primitive3D> primitive) const
{
    return render3D->contains_item(primitive);
}

void RenderEngine::clear_scene()
{
    render2D->clear_items();
    render3D->clear_items();
}

void RenderEngine::clear_3D_scene()
{
    render3D->clear_items();
}

void RenderEngine::clear_2D_scene()
{
    render2D->clear_items();
}

int RenderEngine::num_primitives() const
{
    return num_2D_primitives() + num_3D_primitives();
}

int RenderEngine::num_2D_primitives() const
{
    return render2D->num_items();
}

int RenderEngine::num_3D_primitives() const
{
    return render3D->num_items();
}

void RenderEngine::set_resolution(const Vec2i new_resolution)
{
    surface->resize(new_resolution);
}

void RenderEngine::set_resolution(const int width, const int height)
{
    surface->resize(Vec2i { width, height });
}

Vec2i RenderEngine::get_resolution() const
{
    return surface->get_resolution();
}

void RenderEngine::set_camera_position(const Vec3d position)
{
    render3D->set_camera_position(position);
}

void RenderEngine::set_camera_position(const double x, const double y, const double z)
{
    render3D->set_camera_position(Vec3d { x, y, z });
}

void RenderEngine::set_camera_rotation(const Vec3d rotation_degrees)
{
    render3D->set_camera_rotation(rotation_degrees);
}

void RenderEngine::set_camera_rotation(const double x_deg, const double y_deg, const double z_deg)
{
    render3D->set_camera_rotation(Vec3d { x_deg, y_deg, z_deg });
}

void RenderEngine::set_camera_rotation_degrees(const Vec3d rotation_degrees)
{
    render3D->set_camera_rotation_degrees(rotation_degrees);
}

void RenderEngine::set_camera_rotation_degrees(const double x_deg, const double y_deg, const double z_deg)
{
    render3D->set_camera_rotation_degrees(Vec3d { x_deg, y_deg, z_deg });
}

void RenderEngine::set_camera_fov(const double fov_degrees)
{
    render3D->set_camera_fov(fov_degrees);
}

void RenderEngine::set_camera_fov_degrees(const double fov_degrees)
{
    render3D->set_camera_fov_degrees(fov_degrees);
}

void RenderEngine::set_camera_z_near(const double z_near)
{
    render3D->set_camera_z_near(z_near);
}

void RenderEngine::set_camera_z_far(const double z_far)
{
    render3D->set_camera_z_far(z_far);
}

void RenderEngine::set_light_direction(const Vec3d direction)
{
    render3D->set_light_direction(direction);
}

void RenderEngine::set_light_direction(const double x, const double y, const double z)
{
    render3D->set_light_direction(Vec3d { x, y, z });
}

void RenderEngine::set_ambient_light(const double intensity)
{
    render3D->set_ambient_light(intensity);
}

Vec3d RenderEngine::get_camera_position() const
{
    return render3D->get_camera_position();
}

Vec3d RenderEngine::get_camera_rotation() const
{
    return render3D->get_camera_rotation();
}

double RenderEngine::get_camera_fov() const
{
    return render3D->get_camera_fov();
}

Vec3d RenderEngine::get_camera_forward() const
{
    return render3D->get_camera().get_forward();
}

Vec3d RenderEngine::get_light_direction() const
{
    return render3D->get_light_direction();
}

double RenderEngine::get_ambient_light() const
{
    return render3D->get_ambient_light();
}

void RenderEngine::set_font_directory(const std::filesystem::path &path)
{
    render2D->set_font_directory(path);
}

void RenderEngine::load_font_directory(const std::filesystem::path &path)
{
    render2D->load_font_directory(path);
}

std::filesystem::path RenderEngine::get_font_directory() const
{
    return render2D->get_font_directory();
}

std::shared_ptr<FontTTF> RenderEngine::get_font(const std::string &name) const
{
    return render2D->get_font(name);
}

bool RenderEngine::is_font_loaded(const std::string &name) const
{
    return render2D->is_font_loaded(name);
}

void RenderEngine::set_render_surface(const std::shared_ptr<RenderSurface> new_surface)
{
    surface = new_surface;
    render2D->set_render_surface(new_surface);
    render3D->set_render_surface(new_surface);
}

std::shared_ptr<RenderSurface> RenderEngine::get_render_surface() const
{
    return surface;
}

std::shared_ptr<Render2D> RenderEngine::get_render_2D() const
{
    return render2D;
}

std::shared_ptr<Render3D> RenderEngine::get_render_3D() const
{
    return render3D;
}

void RenderEngine::set_clear_color(const Color4 color)
{
    surface->set_clear_color(color);
}

Color4 RenderEngine::get_clear_color() const
{
    return surface->get_clear_color();
}

std::shared_ptr<Circle2D> RenderEngine::create_circle(const Vec2d position, const double radius, const Color4 color, const double line_thickness) const
{
    return render2D->create_circle(position, radius, color, line_thickness);
}

std::shared_ptr<Circle2D> RenderEngine::create_circle(const double x, const double y, const double radius, const Color4 color, const double line_thickness) const
{
    return create_circle(Vec2d { x, y }, radius, color, line_thickness);
}

std::shared_ptr<Ellipse2D> RenderEngine::create_ellipse(const Vec2d position, const Vec2d radius, const Color4 color, const double line_thickness) const
{
    return render2D->create_ellipse(position, radius, color, line_thickness);
}

std::shared_ptr<Ellipse2D> RenderEngine::create_ellipse(const double x, const double y, const double radius_x, const double radius_y, const Color4 color, const double line_thickness) const
{
    return create_ellipse(Vec2d { x, y }, Vec2d { radius_x, radius_y }, color, line_thickness);
}

std::shared_ptr<Polyline2D> RenderEngine::create_polyline(const Vec2d position, const std::vector<Vec2d> &points, const Color4 color, const double line_thickness) const
{
    return render2D->create_polyline(position, points, color, line_thickness);
}

std::shared_ptr<Polyline2D> RenderEngine::create_polyline(const double x, const double y, const std::vector<Vec2d> &points, const Color4 color, const double line_thickness) const
{
    return create_polyline(Vec2d { x, y }, points, color, line_thickness);
}

std::shared_ptr<Polygon2D> RenderEngine::create_polygon(const Vec2d position, const std::vector<Vec2d> &points, const Color4 color) const
{
    return render2D->create_polygon(position, points, color);
}

std::shared_ptr<Polygon2D> RenderEngine::create_polygon(const double x, const double y, const std::vector<Vec2d> &points, const Color4 color) const
{
    return create_polygon(Vec2d { x, y }, points, color);
}

// std::shared_ptr<Bitmap2D> RenderEngine::create_bitmap(const Vec2d position, const Bitmap &bm) const
// {
//     return render2D->create_bitmap(position, bm);
// }
//
// std::shared_ptr<Bitmap2D> RenderEngine::create_bitmap(const double x, const double y, const Bitmap &bm) const
// {
//     return create_bitmap(Vec2d { x, y }, bm);
// }
//
// std::shared_ptr<Bitmap2D> RenderEngine::create_bitmap(const Vec2d position, const Vec2i resolution) const
// {
//     return render2D->create_bitmap(position, resolution);
// }
//
// std::shared_ptr<Bitmap2D> RenderEngine::create_bitmap(const double x, const double y, const Vec2i resolution) const
// {
//     return create_bitmap(Vec2d { x, y }, resolution);
// }

std::shared_ptr<Text2D> RenderEngine::create_text(const Vec2d position, const std::string &text, const std::shared_ptr<FontTTF> font, const double font_size, const Color4 color) const
{
    return get_render_2D()->create_text(position, text, font, font_size, color);
}

std::shared_ptr<Text2D> RenderEngine::create_text(const double x, const double y, const std::string &text, const std::shared_ptr<FontTTF> font, const double font_size, const Color4 color) const
{
    return create_text(Vec2d { x, y }, text, font, font_size, color);
}

std::shared_ptr<Cuboid3D> RenderEngine::create_cuboid(const Vec3d position, const Vec3d size, const Color4 color, const Shader3D shader) const
{
    return render3D->create_cuboid(position, size, color, shader);
}

std::shared_ptr<Cuboid3D> RenderEngine::create_cuboid(const double x, const double y, const double z, const double size_x, const double size_y, const double size_z, const Color4 color, const Shader3D shader) const
{
    return create_cuboid(Vec3d { x, y, z }, Vec3d { size_x, size_y, size_z }, color, shader);
}

std::shared_ptr<Plane3D> RenderEngine::create_plane(const Vec3d position, const Vec2d size, const Color4 color, const Shader3D shader) const
{
    return render3D->create_plane(position, size, color, shader);
}

std::shared_ptr<Plane3D> RenderEngine::create_plane(const double x, const double y, const double z, const double size_x, const double size_y, const Color4 color, const Shader3D shader) const
{
    return create_plane(Vec3d { x, y, z }, Vec2d { size_x, size_y }, color, shader);
}

std::shared_ptr<Sphere3D> RenderEngine::create_sphere(const Vec3d position, const double radius, const Color4 color, const int segments, const Shader3D shader) const
{
    return render3D->create_sphere(position, radius, color, segments, shader);
}

std::shared_ptr<Sphere3D> RenderEngine::create_sphere(const double x, const double y, const double z, const double radius, const Color4 color, const int segments, const Shader3D shader) const
{
    return create_sphere(Vec3d { x, y, z }, radius, color, segments, shader);
}

}
