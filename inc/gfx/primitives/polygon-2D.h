#pragma once

#include "gfx/core/primitive-2D.h"
#include "gfx/math/box2.h"
#include "gfx/math/vec2.h"
#include "gfx/math/matrix.h"
#include "gfx/geometry/types/barycentric-triangle.h"
#include "gfx/geometry/types/polygon.h"
#include "gfx/geometry/triangulate.h"
#include "gfx/geometry/transform-2D.h"
#include "gfx/geometry/rasterize.h"

namespace gfx
{

class Polygon2D : public PrimitiveTemplate<Polygon2D>
{

public:

    Box2d get_geometry_size() const override;

    bool point_collides(const Vec2d point, const Matrix3x3d &transform) const override;

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

    template<typename EmitPixel>
    void rasterize(const Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        for (const auto &component : components)
        {
            rasterize_component(component, transform, emit_pixel);
        }
    }

private:

    bool cache_clockwise(const int component);
    bool cache_clockwise_hole(const int component, const int hole);

    template<typename EmitPixel>
    void rasterize_component(const Polygon::Component &component, const Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        std::vector<Polygon::Contour> transformed_holes;
        for (const auto &hole : component.holes)
        {
            transformed_holes.push_back(Polygon::Contour { 
                Transform2D::transform_points(hole.vertices, transform),
                hole.clockwise 
            });
        }

        Polygon::Component transformed_component { 
            Transform2D::transform_points(component.contour.vertices, transform),
            component.contour.clockwise,
            transformed_holes
        };

        std::vector<BarycentricTriangle> triangles { 
            Triangulate::triangulate_polygon(transformed_component)
        };

        for (auto triangle : triangles)
        {
            Rasterize::rasterize_filled_triangle(triangle, color, emit_pixel);
        }
    }
    std::vector<Polygon::Component> components;
    static constexpr int CORNER_SEGMENTS = 8;
};

};
