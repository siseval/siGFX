#include "gfx/primitives/polygon-2D.h"

#include "gfx/geometry/transform-2D.h"
#include "gfx/geometry/triangulate.h"
#include "gfx/geometry/rasterize.h"
#include "gfx/geometry/types/barycentric-triangle.h"


namespace gfx
{

std::vector<Vec2i> Polygon2D::rasterize(const Matrix3x3d &transform) const 
{
    std::vector<Vec2i> pixels;

    for (const auto &component : components)
    {
        rasterize_component(component, transform, pixels);
    }

    return pixels;
}

Box2d Polygon2D::get_geometry_size() const
{
    Box2d bounds { Vec2d::zero(), Vec2d::zero() };

    for (auto component : components)
    {
        for (auto point : component.contour.vertices)
        {
            bounds.expand(point);
        }
        for (auto hole : component.holes)
        {
            for (auto point : hole.vertices)
            {
                bounds.expand(point);
            }
        }
    }

    return bounds;
}

bool Polygon2D::cache_clockwise(const int component)
{
    Polygon::Contour &contour = components[component].contour;
    double sum = 0.0;
    for (int i = 0; i < contour.vertices.size(); ++i)
    {
        Vec2d p0 { contour.vertices[i] };
        Vec2d p1 { contour.vertices[(i + 1) % contour.vertices.size()] };
        sum += (p1.x - p0.x) * (p1.y + p0.y);
    }
    contour.clockwise = sum < 0.0;
    return contour.clockwise;
}

bool Polygon2D::cache_clockwise_hole(const int component, const int hole)
{
    Polygon::Contour &contour = components[component].holes[hole];
    double sum = 0.0;
    for (int i = 0; i < contour.vertices.size(); ++i)
    {
        Vec2d p0 { contour.vertices[i] };
        Vec2d p1 { contour.vertices[(i + 1) % contour.vertices.size()] };
        sum += (p1.x - p0.x) * (p1.y + p0.y);
    }
    contour.clockwise = sum < 0.0;
    return contour.clockwise;
}

void Polygon2D::add_vertex(const Vec2d vertex, const int component) 
{ 
    if (component >= components.size()) 
    { 
        components.resize(component + 1); 
    }
    components[component].contour.vertices.push_back(vertex); 
    cache_clockwise(component); 
    set_obb_dirty(); 
}

void Polygon2D::add_vertices(const std::vector<Vec2d> &new_vertices, const int component)
{ 
    if (component >= components.size()) 
    { 
        components.resize(component + 1); 
    }
    std::vector<Vec2d> &points { components[component].contour.vertices };
    points.insert(points.end(), new_vertices.begin(), new_vertices.end()); 
    cache_clockwise(component); 
    set_obb_dirty(); 
}

void Polygon2D::set_vertex(const size_t index, const Vec2d vertex, const int component)
{ 
    if (component >= components.size()) 
    { 
        components.resize(component + 1); 
    }
    std::vector<Vec2d> &points { components[component].contour.vertices };
    if (index < points.size()) 
    { 
        points[index] = vertex; 
        cache_clockwise(component);
        set_obb_dirty();
    } 
}

void Polygon2D::set_vertices(const std::vector<Vec2d> &new_vertices, const int component)
{ 
    if (component >= components.size()) 
    { 
        components.resize(component + 1); 
    }
    components[component].contour.vertices = new_vertices; 
    cache_clockwise(component); 
    set_obb_dirty(); 
}

void Polygon2D::clear_vertices(const int component)
{ 
    if (component >= components.size()) 
    { 
        return;
    }
    components[component].contour.vertices.clear(); 
    set_obb_dirty(); 
}

std::vector<Vec2d> Polygon2D::get_vertices(const int component) const 
{ 
    if (component >= components.size())
    { 
        return {};
    }
    return components[component].contour.vertices;
}

void Polygon2D::add_hole_vertex(const Vec2d vertex, const int component, const int hole) 
{ 
    if (component >= components.size()) 
    { 
        components.resize(component + 1); 
    }
    components[component].holes[hole].vertices.push_back(vertex); 
    cache_clockwise_hole(component, hole); 
    set_obb_dirty(); 
}

void Polygon2D::add_hole_vertices(const std::vector<Vec2d> &new_vertices, const int component, const int hole)
{ 
    if (component >= components.size()) 
    { 
        components.resize(component + 1); 
    }
    if (hole >= components[component].holes.size())
    {
        components[component].holes.resize(hole + 1);
    }
    std::vector<Vec2d> &points { components[component].holes[hole].vertices };
    points.insert(points.end(), new_vertices.begin(), new_vertices.end()); 
    cache_clockwise_hole(component, hole); 
    set_obb_dirty(); 
}

void Polygon2D::set_hole_vertex(const size_t index, const Vec2d vertex, const int component, const int hole)
{ 
    if (component >= components.size()) 
    { 
        components.resize(component + 1); 
    }
    if (hole >= components[component].holes.size())
    {
        components[component].holes.resize(hole + 1);
    }
    std::vector<Vec2d> &points { components[component].holes[hole].vertices };
    if (component < points.size()) 
    { 
        points[index] = vertex; 
        cache_clockwise_hole(component, hole); 
        set_obb_dirty();
    } 
}

void Polygon2D::set_hole_vertices(const std::vector<Vec2d> &new_vertices, const int component, const int hole)
{ 
    if (component >= components.size()) 
    { 
        components.resize(component + 1); 
    }
    if (hole >= components[component].holes.size())
    {
        components[component].holes.resize(hole + 1);
    }
    components[component].holes[hole].vertices = new_vertices;
    cache_clockwise_hole(component, hole); 
    set_obb_dirty(); 
}

void Polygon2D::clear_hole_vertices(const int component, const int hole)
{ 
    if (component >= components.size()) 
    { 
        return;
    }
    if (hole >= components[component].holes.size())
    {
        return;
    }
    components[component].holes[hole].vertices.clear(); 
    set_obb_dirty(); 
}

std::vector<Vec2d> Polygon2D::get_hole_vertices(const int component, const int hole) const 
{ 
    if (component >= components.size())
    { 
        return {};
    }
    if (hole >= components[component].holes.size())
    {
        return {};
    }
    return components[component].holes[hole].vertices;
}

void Polygon2D::rasterize_component(const Polygon::Component &component, const Matrix3x3d &transform, std::vector<Vec2i> &pixels) const
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
        Rasterize::rasterize_filled_triangle(triangle, pixels);
    }
}



}
