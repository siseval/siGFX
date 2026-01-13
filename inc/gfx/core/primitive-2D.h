#pragma once

#include "gfx/core/types/color4.h"
#include "gfx/core/types/obb-2D.h"
#include "gfx/core/shader-2D.h"
#include "gfx/shaders/default-shader-2D.h"
#include "gfx/core/types/uuid.h"
#include "gfx/math/box2.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Primitive2D
{

public:

    Primitive2D();

    virtual std::vector<Vec2i> rasterize(const Matrix3x3d& transform) const = 0;

    virtual Box2d get_geometry_size() const = 0;
    virtual Box2d get_axis_aligned_bounding_box(const Matrix3x3d &transform) const;

    OBB2D get_oriented_bounding_box(const Matrix3x3d &transform) const;

    void set_position(const Vec2d pos);
    void set_position(const double x, const double y);
    void set_scale(const Vec2d s);
    void set_scale(const double sx, const double sy);
    void set_scale(const double s);
    void set_rotation(const double r);
    void set_rotation_degrees(const double r);
    void set_color(const Color4 col);
    void set_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255);
    void set_color(const double r, const double g, const double b, const double a = 1.0);
    void set_anchor(const Vec2d pos);
    void set_anchor(const double x, const double y);
    void set_depth(const int d);
    void set_visible(const bool v);
    void set_shader(const std::shared_ptr<Shader2D> &shd);

    Vec2d get_position() const;
    Vec2f get_scale() const;
    double get_rotation() const;
    double get_rotation_degrees() const;
    Color4 get_color() const;
    Vec2d get_anchor() const;
    int get_depth() const;
    bool is_visible() const;
    std::shared_ptr<Shader2D> get_shader() const;

    UUID get_id() const;
    Vec2d get_uv(const Vec2d point) const;
    Matrix3x3d get_transform() const;

    int64_t get_transform_version() const; 
    void increment_transform_version();

protected:

    void set_obb_dirty();
    void set_transform_dirty();

    bool is_transform_dirty() const;
    bool is_obb_dirty() const;

    UUID id;
    std::shared_ptr<Shader2D> shader = std::make_shared<DefaultShader2D>();

    Color4 color { Color4::white() };

    Vec2d position { 0.0, 0.0 };
    Vec2d anchor { 0.0, 0.0 };
    Vec2d scale { 1.0, 1.0 };

    bool fill = false;
    bool visible = true;
    double rotation = 0.0;
    int depth = 0;

    mutable OBB2D cached_obb;
    mutable bool obb_dirty = true;

    mutable Matrix3x3d cached_transform;
    mutable bool transform_dirty = true;
    int64_t transform_version = -1;
};

};

template <>
struct std::hash<gfx::Primitive2D>
{
    size_t operator()(const gfx::Primitive2D& item) const
    {
        int64_t hash = std::hash<gfx::Vec2d>()(item.get_position());
        hash ^= (std::hash<gfx::Vec2d>()(item.get_scale()) << 1);
        hash ^= (std::hash<double>()(item.get_rotation()) << 1);
        hash ^= (std::hash<int>()(item.get_depth()) << 1);

        return hash;
    }
};
