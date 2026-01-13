#include "gfx/core/primitive-2D.h"

#include "gfx/geometry/transform-2D.h"

#include <algorithm>


namespace gfx
{

Primitive2D::Primitive2D() : id(UUID::generate()) {}

Box2d Primitive2D::get_axis_aligned_bounding_box(const Matrix3x3d &transform) const
{
    Box2d extent { get_geometry_size() };
    Vec2d top_left { extent.min };
    Vec2d bot_right { extent.max };

    std::vector<Vec2d> corners {
        { top_left.x, top_left.y },
        { bot_right.x, top_left.y },
        { top_left.x, bot_right.y },
        { bot_right.x, bot_right.y },
    };
    std::vector<Vec2d> transformed_corners { Transform2D::transform_points(corners, transform) };

    Box2d bounds { transformed_corners[0], transformed_corners[0] };
    bounds.expand(transformed_corners);

    return bounds;
}

OBB2D Primitive2D::get_oriented_bounding_box(const Matrix3x3d &transform) const
{
    if (!obb_dirty)
    {
        return cached_obb;
    }

    Box2d extent { get_geometry_size() };

    OBB2D bounds {
        extent.min,
        { extent.max.x - extent.min.x, 0 },
        { 0, extent.max.y - extent.min.y }
    };
   
    bounds.origin = Transform2D::transform_point(bounds.origin, transform);
    bounds.side_x = Transform2D::transform_vector(bounds.side_x, transform);
    bounds.side_y = Transform2D::transform_vector(bounds.side_y, transform);

    cached_obb = bounds;
    obb_dirty = false;

    return bounds;
}

void Primitive2D::set_position(const Vec2d pos) 
{ 
    position = pos; 
    increment_transform_version(); 
    Primitive2D::set_obb_dirty(); 
    Primitive2D::set_transform_dirty();
}

void Primitive2D::set_position(const double x, const double y) 
{ 
    position = Vec2d { x, y }; 
    increment_transform_version(); 
    Primitive2D::set_obb_dirty(); 
    Primitive2D::set_transform_dirty();
}

void Primitive2D::set_scale(const Vec2d s) 
{ 
    scale = s; 
    increment_transform_version(); 
    Primitive2D::set_obb_dirty(); 
    Primitive2D::set_transform_dirty();
}

void Primitive2D::set_scale(const double sx, const double sy) 
{ 
    scale = Vec2d { sx, sy }; 
    increment_transform_version(); 
    Primitive2D::set_obb_dirty(); 
    Primitive2D::set_transform_dirty();
}

void Primitive2D::set_scale(const double s) 
{ 
    scale = Vec2d { s, s }; 
    increment_transform_version(); 
    Primitive2D::set_obb_dirty(); 
    Primitive2D::set_transform_dirty();
}

void Primitive2D::set_rotation(const double r) 
{ 
    rotation = r; 
    increment_transform_version(); 
    Primitive2D::set_obb_dirty(); 
    Primitive2D::set_transform_dirty();
}

void Primitive2D::set_rotation_degrees(const double r) 
{ 
    rotation = r * std::numbers::pi / 180; 
    increment_transform_version(); 
    Primitive2D::set_obb_dirty(); 
    Primitive2D::set_transform_dirty();
}

void Primitive2D::set_color(const Color4 col) 
{
    color = col; 
}

void Primitive2D::set_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) 
{ 
    color = Color4 { r, g, b, a }; 
}

void Primitive2D::set_color(const double r, const double g, const double b, const double a) 
{ 
    color = Color4 { 
        static_cast<uint8_t>(std::clamp(r * 255.0, 0.0, 255.0)), 
        static_cast<uint8_t>(std::clamp(g * 255.0, 0.0, 255.0)), 
        static_cast<uint8_t>(std::clamp(b * 255.0, 0.0, 255.0)), 
        static_cast<uint8_t>(std::clamp(a * 255.0, 0.0, 255.0)) 
    }; 
}

void Primitive2D::set_anchor(const Vec2d pos) 
{ 
    anchor = pos; 
    increment_transform_version(); 
    Primitive2D::set_obb_dirty();
    Primitive2D::set_transform_dirty();
}

void Primitive2D::set_anchor(const double x, const double y) 
{ 
    anchor = Vec2d { x, y }; 
    increment_transform_version(); 
    Primitive2D::set_obb_dirty();
    Primitive2D::set_transform_dirty();
}

void Primitive2D::set_depth(const int d) 
{ 
    depth = d; 
}

void Primitive2D::set_visible(const bool v) 
{ 
    visible = v; 
}

void Primitive2D::set_shader(const std::shared_ptr<Shader2D> &shd) 
{ 
    shader = shd; 
}

Vec2d Primitive2D::get_position() const 
{ 
    return position; 
}

Vec2f Primitive2D::get_scale() const 
{ 
    return scale; 
}

double Primitive2D::get_rotation() const 
{ 
    return rotation; 
}

double Primitive2D::get_rotation_degrees() const 
{ 
    return rotation * 180 / std::numbers::pi; 
}

Color4 Primitive2D::get_color() const 
{ 
    return color; 
}

Vec2d Primitive2D::get_anchor() const 
{ 
    return anchor; 
}

int Primitive2D::get_depth() const 
{ 
    return depth; 
}

bool Primitive2D::is_visible() const 
{ 
    return visible; 
}

std::shared_ptr<Shader2D> Primitive2D::get_shader() const 
{ 
    return shader; 
}

UUID Primitive2D::get_id() const 
{ 
    return id; 
}

Vec2d Primitive2D::get_uv(const Vec2d point) const
{
    OBB2D obb { get_oriented_bounding_box(get_transform()) };
    return obb.get_uv(point);
}

Matrix3x3d Primitive2D::get_transform() const
{
    if (!transform_dirty)
    {
        return cached_transform;
    }

    Vec2d size { get_geometry_size().size() };
    Vec2d anchor_offset { get_anchor() * size };

    Matrix3x3d anchor_translation_matrix { Transform2D::translate(-anchor_offset) };
    Matrix3x3d scale_matrix { Transform2D::scale(scale) };
    Matrix3x3d rotation_matrix { Transform2D::rotate(rotation) };
    Matrix3x3d position_translation_matrix { Transform2D::translate(get_position()) };

    cached_transform = 
        position_translation_matrix * rotation_matrix * 
        scale_matrix * anchor_translation_matrix;

    cached_transform = position_translation_matrix * rotation_matrix * scale_matrix * anchor_translation_matrix;
    transform_dirty = false;

    return cached_transform;
}

int64_t Primitive2D::get_transform_version() const 
{ 
    return transform_version; 
}

void Primitive2D::increment_transform_version() 
{ 
    transform_version++; 
}

void Primitive2D::set_obb_dirty() 
{ 
    obb_dirty = true; 
}

void Primitive2D::set_transform_dirty() 
{ 
    transform_dirty = true; 
}

bool Primitive2D::is_obb_dirty() const 
{ 
    return obb_dirty; 
}

bool Primitive2D::is_transform_dirty() const 
{ 
    return transform_dirty; 
}

}


