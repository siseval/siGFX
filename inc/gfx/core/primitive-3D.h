#pragma once

#include "gfx/core/types/uuid.h"
#include "gfx/core/shader-3D.h"
#include "gfx/math/matrix.h"
#include "gfx/math/box3.h"
#include "gfx/geometry/polygon-mesh.h"
#include "gfx/shaders/default-shader-3D.h"

namespace gfx
{

class Primitive3D
{

public:

    Primitive3D() : id(UUID::generate()), shader(DefaultShader3D()) {}

    UUID get_id() const
    {
        return id;
    }

    virtual const PolygonMesh& get_mesh() const = 0;

    Matrix4x4d get_transform() const;
    Box3d get_geometry_size() const;

    void set_position(const Vec3d &pos)
    {
        position = pos;
        set_transform_dirty(true);
        increment_transform_version();
    }

    void set_position(const double x, const double y, const double z)
    {
        position = Vec3d { x, y, z };
        set_transform_dirty(true);
        increment_transform_version();
    }

    Vec3d get_position() const
    {
        return position;
    }

    void set_rotation(const Vec3d &rot)
    {
        rotation = rot;
        set_transform_dirty(true);
        increment_transform_version();
    }

    void set_rotation(const double x, const double y, const double z)
    {
        rotation = Vec3d { x, y, z };
        set_transform_dirty(true);
        increment_transform_version();
    }

    void set_rotation_degrees(const Vec3d &rot)
    {
        rotation = Vec3d { rot.x * std::numbers::pi / 180, rot.y * std::numbers::pi / 180, rot.z * std::numbers::pi / 180 };
        set_transform_dirty(true);
        increment_transform_version();
    }

    void set_rotation_degrees(const double x, const double y, const double z)
    {
        rotation = Vec3d { x * std::numbers::pi / 180, y * std::numbers::pi / 180, z * std::numbers::pi / 180 };
        set_transform_dirty(true);
        increment_transform_version();
    }

    Vec3d get_rotation() const
    {
        return rotation;
    }

    Vec3d get_rotation_degrees() const
    {
        return Vec3d { rotation.x * 180 / std::numbers::pi, rotation.y * 180 / std::numbers::pi, rotation.z * 180 / std::numbers::pi };
    }

    void set_scale(const Vec3d &s)
    {
        scale = s;
        set_transform_dirty(true);
        increment_transform_version();
    }

    void set_scale(const double x, const double y, const double z)
    {
        scale = Vec3d { x, y, z };
        set_transform_dirty(true);
        increment_transform_version();
    }

    Vec3d get_scale() const
    {
        return scale;
    }

    void set_anchor(const Vec3d &a)
    {
        anchor = a;
        set_transform_dirty(true);
        increment_transform_version();
    }

    Vec3d get_anchor() const
    {
        return anchor;
    } 

    Shader3D get_shader() const
    {
        return shader;
    }

    void set_shader(const Shader3D shd)
    {
        shader = shd;
    }

    int64_t get_transform_version() const
    {
        return transform_version;
    }

    Color4 get_color() const
    {
        return color;
    }

    void set_color(const Color4 &col)
    {
        color = col;
    }

protected:

    void set_mesh_dirty(const bool dirty) const
    {
        mesh_dirty = dirty;
    }

    void set_transform_dirty(const bool dirty) const
    {
        transform_dirty = dirty;
    }

    bool is_mesh_dirty() const
    {
        return mesh_dirty;
    }

    bool is_transform_dirty() const
    {
        return transform_dirty;
    }

    void increment_transform_version()
    {
        transform_version++;
    }

    UUID id;
    Shader3D shader;
    Color4 color;

    Vec3d position;
    Vec3d rotation;
    Vec3d scale { 1.0, 1.0, 1.0 };
    Vec3d anchor;

    mutable PolygonMesh mesh_data;
    mutable bool mesh_dirty = true;

    mutable Matrix4x4d cached_transform;
    mutable bool transform_dirty = true;
    int64_t transform_version = -1;

};

}
