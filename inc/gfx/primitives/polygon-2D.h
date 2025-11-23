#ifndef POLYGON_2D_H
#define POLYGON_2D_H

#include <gfx/core/render-surface.h>
#include <gfx/core/primitive-2D.h>
#include <gfx/math/box2.h>
#include <gfx/math/vec2.h>
#include <gfx/math/matrix.h>
#include <gfx/geometry/triangle.h>
#include <gfx/geometry/types/polygon.h>
#include <gfx/geometry/rasterize.h>
#include <gfx/geometry/triangulate.h>
#include <gfx/utils/transform.h>

namespace gfx::primitives
{

class Polygon2D : public gfx::core::PrimitiveTemplate<Polygon2D>
{

public:

    gfx::math::Box2d get_geometry_size() const override;

    bool point_collides(const gfx::math::Vec2d point, const gfx::math::Matrix3x3d &transform) const override;

    void add_vertex(const gfx::math::Vec2d vertex, const int component = 0);
    void add_vertices(const std::vector<gfx::math::Vec2d> &new_vertices, const int component = 0);

    void set_vertex(const size_t index, const gfx::math::Vec2d vertex, const int component = 0);
    void set_vertices(const std::vector<gfx::math::Vec2d> &new_vertices, const int component = 0);

    void clear_vertices(const int component = 0);

    std::vector<gfx::math::Vec2d> get_vertices(const int component = 0) const;

    void add_hole_vertex(const gfx::math::Vec2d vertex, const int component = 0, const int hole = 0);
    void add_hole_vertices(const std::vector<gfx::math::Vec2d> &new_vertices, const int component = 0, const int hole = 0);

    void set_hole_vertex(const size_t index, const gfx::math::Vec2d vertex, const int component = 0, const int hole = 0);
    void set_hole_vertices(const std::vector<gfx::math::Vec2d> &new_vertices, const int component = 0, const int hole = 0);

    void clear_hole_vertices(const int component = 0, const int hole = 0);

    std::vector<gfx::math::Vec2d> get_hole_vertices(const int component = 0, const int hole = 0) const;

    template<typename EmitPixel>
    void rasterize(const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
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
    void rasterize_component(const geometry::types::Component &component, const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        std::vector<geometry::types::Contour> transformed_holes;
        for (const auto &hole : component.holes)
        {
            transformed_holes.push_back(geometry::types::Contour { 
                utils::transform_points(hole.vertices, transform),
                hole.clockwise 
            });
        }

        gfx::geometry::types::Component transformed_component { 
            utils::transform_points(component.contour.vertices, transform),
            component.contour.clockwise,
            transformed_holes
        };

        std::vector<gfx::geometry::Triangle> triangles { 
            geometry::triangulate_polygon(transformed_component)
        };

        for (auto triangle : triangles)
        {
            geometry::rasterize_filled_triangle(triangle, color, emit_pixel);
        }
    }
    std::vector<gfx::geometry::types::Component> components;
    static constexpr int CORNER_SEGMENTS = 8;
};

};

#endif // POLYGON_2D_H
