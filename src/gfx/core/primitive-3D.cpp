#include "gfx/core/primitive-3D.h"

#include "gfx/geometry/transform-3D.h"

namespace gfx
{

Primitive3D::Primitive3D() : id(UUID::generate()), shader(DefaultShader3D()) {}

void Primitive3D::set_position(const Vec3d &pos)
{
    position = pos;
    set_transform_dirty(true);
    increment_transform_version();
}

void Primitive3D::set_position(const double x, const double y, const double z)
{
    position = Vec3d { x, y, z };
    set_transform_dirty(true);
    increment_transform_version();
}

void Primitive3D::set_scale(const Vec3d &s)
{
    scale = s;
    set_transform_dirty(true);
    increment_transform_version();
}

void Primitive3D::set_scale(const double x, const double y, const double z)
{
    scale = Vec3d { x, y, z };
    set_transform_dirty(true);
    increment_transform_version();
}

void Primitive3D::set_rotation(const Vec3d &rot)
{
    rotation = rot;
    set_transform_dirty(true);
    increment_transform_version();
}

void Primitive3D::set_rotation(const double x, const double y, const double z)
{
    rotation = Vec3d { x, y, z };
    set_transform_dirty(true);
    increment_transform_version();
}

void Primitive3D::set_rotation_degrees(const Vec3d &rot)
{
    rotation = Vec3d { rot.x * std::numbers::pi / 180, rot.y * std::numbers::pi / 180, rot.z * std::numbers::pi / 180 };
    set_transform_dirty(true);
    increment_transform_version();
}

void Primitive3D::set_rotation_degrees(const double x, const double y, const double z)
{
    rotation = Vec3d { x * std::numbers::pi / 180, y * std::numbers::pi / 180, z * std::numbers::pi / 180 };
    set_transform_dirty(true);
    increment_transform_version();
}

void Primitive3D::set_color(const Color4 &col)
{
    color = col;
}

void Primitive3D::set_anchor(const Vec3d &a)
{
    anchor = a;
    set_transform_dirty(true);
    increment_transform_version();
}

void Primitive3D::set_shader(const Shader3D shd)
{
    shader = shd;
}

Vec3d Primitive3D::get_position() const
{
    return position;
}

Vec3d Primitive3D::get_scale() const
{
    return scale;
}

Vec3d Primitive3D::get_rotation() const
{
    return rotation;
}

Vec3d Primitive3D::get_rotation_degrees() const
{
    return Vec3d { rotation.x * 180 / std::numbers::pi, rotation.y * 180 / std::numbers::pi, rotation.z * 180 / std::numbers::pi };
}

Color4 Primitive3D::get_color() const
{
    return color;
}

Vec3d Primitive3D::get_anchor() const
{
    return anchor;
}

Shader3D Primitive3D::get_shader() const
{
    return shader;
}

UUID Primitive3D::get_id() const
{
    return id;
}

Matrix4x4d Primitive3D::get_transform() const
{
    if (!transform_dirty)
    {
        return cached_transform;
    }

    const Vec3d anchor_offset { get_anchor() * get_geometry_size().size() };

    const Matrix4x4d anchor_translation = Transform3D::translate({ -anchor_offset.x, -anchor_offset.y, -anchor_offset.z });
    const Matrix4x4d translation = Transform3D::translate({ position.x, position.y, position.z });

    const Matrix4x4d rotation_x = Transform3D::rotate_x(rotation.x);
    const Matrix4x4d rotation_y = Transform3D::rotate_y(rotation.y);
    const Matrix4x4d rotation_z = Transform3D::rotate_z(rotation.z);
    const Matrix4x4d rotation = rotation_z * rotation_y * rotation_x;

    const Matrix4x4d scaling = Transform3D::scale({ scale.x, scale.y, scale.z });

    cached_transform = translation * rotation * scaling * anchor_translation;
    transform_dirty = false;

    return cached_transform;
}

Box3d Primitive3D::get_geometry_size() const
{
    return get_mesh().get_extent();
}

int64_t Primitive3D::get_transform_version() const
{
    return transform_version;
}

void Primitive3D::set_mesh_dirty(const bool dirty) const
{
    mesh_dirty = dirty;
}

void Primitive3D::set_transform_dirty(const bool dirty) const
{
    transform_dirty = dirty;
}

bool Primitive3D::is_mesh_dirty() const
{
    return mesh_dirty;
}

bool Primitive3D::is_transform_dirty() const
{
    return transform_dirty;
}

void Primitive3D::increment_transform_version()
{
    transform_version++;
}



}
