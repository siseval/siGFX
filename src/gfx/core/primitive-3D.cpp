#include "gfx/core/primitive-3D.h"

#include "gfx/geometry/transform-3D.h"

namespace gfx
{

Matrix4x4d Primitive3D::get_transform() const
{
    if (!transform_dirty)
    {
        return cached_transform;
    }

    Vec3d anchor_offset { get_anchor() * get_geometry_size().size() };

    Matrix4x4d anchor_translation = Transform3D::translate({ -anchor_offset.x, -anchor_offset.y, -anchor_offset.z });
    Matrix4x4d translation = Transform3D::translate({ position.x, position.y, position.z });

    Matrix4x4d rotation_x = Transform3D::rotate_x(rotation.x);
    Matrix4x4d rotation_y = Transform3D::rotate_y(rotation.y);
    Matrix4x4d rotation_z = Transform3D::rotate_z(rotation.z);
    Matrix4x4d rotation = rotation_z * rotation_y * rotation_x;

    Matrix4x4d scaling = Transform3D::scale({ scale.x, scale.y, scale.z });

    cached_transform = translation * rotation * scaling * anchor_translation;
    transform_dirty = false;

    return cached_transform;
}

Box3d Primitive3D::get_geometry_size() const
{
    return get_mesh().get_extent();
}

}
