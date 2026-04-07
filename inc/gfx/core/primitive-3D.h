#pragma once

#include "gfx/core/material/material.h"
#include "gfx/core/types/uuid.h"
#include "gfx/core/types/polygon-mesh.h"
#include "gfx/math/box3.h"
#include "gfx/math/matrix.h"

namespace gfx
{
class Primitive3D
{

public:

    virtual ~Primitive3D() = default;

    Primitive3D();

    virtual const PolygonMesh &get_mesh() const = 0;

    void set_position(const Vec3d &pos);
    void set_position(double x, double y, double z);
    void set_scale(const Vec3d &s);
    void set_scale(double x, double y, double z);
    void set_rotation(const Vec3d &rot);
    void set_rotation(double x, double y, double z);
    void set_rotation_degrees(const Vec3d &rot);
    void set_rotation_degrees(double x, double y, double z);
    void set_color(const Color4 &col);
    void set_color(const double r, const double g, const double b, const double a = 1.0);
    void set_anchor(const Vec3d &a);
    void set_material(const Material mat);

    Vec3d get_position() const;
    Vec3d get_scale() const;
    Vec3d get_rotation() const;
    Vec3d get_rotation_degrees() const;
    Color4 get_color() const;
    Vec3d get_anchor() const;
    Material get_material() const;

    virtual Box3d get_aabb() const;
    virtual BoundingSphere get_bounding_sphere() const;

    UUID get_id() const;
    Matrix4x4d get_transform() const;
    int64_t get_transform_version() const;

protected:

    void set_mesh_dirty(bool dirty) const;
    void set_transform_dirty(bool dirty) const;

    bool is_mesh_dirty() const;
    bool is_transform_dirty() const;

    void increment_transform_version();

    UUID id;
    Material material { Material() };
    Color4 color;

    Vec3d position;
    Vec3d rotation;
    Vec3d scale { 1.0, 1.0, 1.0 };
    Vec3d anchor;

    mutable PolygonMesh mesh_data;
    mutable bool mesh_dirty { true };

    mutable Matrix4x4d cached_transform;
    mutable bool transform_dirty { true };
    int64_t transform_version { 0 };
};
}
