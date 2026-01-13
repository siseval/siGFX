#pragma once

#include "gfx/core/types/uuid.h"
#include "gfx/core/shader-3D.h"
#include "gfx/math/matrix.h"
#include "gfx/math/box3.h"
#include "gfx/geometry/types/polygon-mesh.h"
#include "gfx/shaders/default-shader-3D.h"

namespace gfx
{

class Primitive3D
{

public:

    Primitive3D();

    virtual const PolygonMesh& get_mesh() const = 0;

    void set_position(const Vec3d &pos);
    void set_position(const double x, const double y, const double z);
    void set_scale(const Vec3d &s);
    void set_scale(const double x, const double y, const double z);
    void set_rotation(const Vec3d &rot);
    void set_rotation(const double x, const double y, const double z);
    void set_rotation_degrees(const Vec3d &rot);
    void set_rotation_degrees(const double x, const double y, const double z);
    void set_color(const Color4 &col);
    void set_anchor(const Vec3d &a);
    void set_shader(const Shader3D shd);

    Vec3d get_position() const;
    Vec3d get_scale() const;
    Vec3d get_rotation() const;
    Vec3d get_rotation_degrees() const;
    Color4 get_color() const;
    Vec3d get_anchor() const;
    Shader3D get_shader() const;

    UUID get_id() const;
    Matrix4x4d get_transform() const;
    Box3d get_geometry_size() const;
    int64_t get_transform_version() const;

protected:

    void set_mesh_dirty(const bool dirty) const;
    void set_transform_dirty(const bool dirty) const;

    bool is_mesh_dirty() const;
    bool is_transform_dirty() const;

    void increment_transform_version();

    UUID id;
    Shader3D shader { DefaultShader3D() };
    Color4 color { Color4::white() };

    Vec3d position { 1.0, 1.0, 1.0 };
    Vec3d rotation { 0.0, 0.0, 0.0 };
    Vec3d scale { 1.0, 1.0, 1.0 };
    Vec3d anchor { 0.0, 0.0, 0.0 };

    mutable PolygonMesh mesh_data;
    mutable bool mesh_dirty = true;

    mutable Matrix4x4d cached_transform;
    mutable bool transform_dirty = true;
    int64_t transform_version = -1;

};

}
