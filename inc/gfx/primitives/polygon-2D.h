#pragma once

#include "gfx/core/primitive-2D.h"
#include "gfx/math/box2.h"
#include "gfx/math/vec2.h"
#include "gfx/math/matrix.h"
#include "gfx/geometry/types/polygon.h"

namespace gfx
{

class Polygon2D : public Primitive2D
{

public:

    Box2d get_geometry_size() const override;

    void add_vertex(const Vec2d vertex, const int component = 0);
    void add_vertices(const std::vector<Vec2d> &new_vertices, const int component = 0);

    void set_vertex(const size_t index, const Vec2d vertex, const int component = 0);
    void set_vertices(const std::vector<Vec2d> &new_vertices, const int component = 0);

    void clear_vertices(const int component = 0);

    std::vector<Vec2d> get_vertices(const int component = 0) const;

    void add_hole_vertex(const Vec2d vertex, const int component = 0, const int hole = 0);
    void add_hole_vertices(const std::vector<Vec2d> &new_vertices, const int component = 0, const int hole = 0);

    void set_hole_vertex(const size_t index, const Vec2d vertex, const int component = 0, const int hole = 0);
    void set_hole_vertices(const std::vector<Vec2d> &new_vertices, const int component = 0, const int hole = 0);

    void clear_hole_vertices(const int component = 0, const int hole = 0);

    std::vector<Vec2d> get_hole_vertices(const int component = 0, const int hole = 0) const;

    std::vector<Vec2i> rasterize(const Matrix3x3d &transform) const override;

private:

    bool cache_clockwise(const int component);
    bool cache_clockwise_hole(const int component, const int hole);

    void rasterize_component(const Polygon::Component &component, const Matrix3x3d &transform, std::vector<Vec2i> &pixels) const;

    std::vector<Polygon::Component> components;
    static constexpr int CORNER_SEGMENTS = 8;
};

};
