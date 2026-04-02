#include "gfx/primitives/cone-3D.h"

namespace gfx
{


const PolygonMesh &Cone3D::get_mesh() const
{
    if (!is_mesh_dirty())
    {
        return mesh_data;
    }

    mesh_data.clear();

    const double r { radius * 0.5 };
    const double h { height * 0.5 };

    constexpr int segments { 32 };
    constexpr double two_pi { std::numbers::pi * 2 };

    std::vector<Vec3d> vertices;
    std::vector<Vec3d> normals;
    std::vector<size_t> indices;

    for (int i = 0; i < segments; ++i)
    {
        const double a0 { (i + 0) * two_pi / segments };
        const double a1 { (i + 1) * two_pi / segments };

        const Vec3d v0 { r * std::cos(a0), -h, r * std::sin(a0) };
        const Vec3d v1 { r * std::cos(a1), -h, r * std::sin(a1) };
        const Vec3d tip { 0, h, 0 };

        const Vec3d n { Vec3d::cross((v1 - v0), tip - v0).normalize() };

        const size_t base { vertices.size() };

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(tip);

        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);

        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
    }

    for (int i = 0; i < segments; ++i)
    {
        const double a0 { (i + 0) * two_pi / segments };
        const double a1 { (i + 1) * two_pi / segments };

        const Vec3d v0 { r * std::cos(a1), -h, r * std::sin(a1) };
        const Vec3d v1 { r * std::cos(a0), -h, r * std::sin(a0) };
        const Vec3d center { 0, -h, 0 };

        const Vec3d n { 0, -1, 0 };

        const size_t base { vertices.size() };

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(center);

        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);

        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
    }

    mesh_data.set_vertices(std::move(vertices));
    mesh_data.set_normals(std::move(normals));
    mesh_data.set_indices(std::move(indices));
    mesh_data.set_colors(std::vector(mesh_data.num_vertices(), get_color()));

    set_mesh_dirty(false);

    return mesh_data;
}

void Cone3D::set_radius(const double new_radius)
{
    radius = new_radius;
}

void Cone3D::set_height(const double new_height)
{
    height = new_height;
}

void Cone3D::set_segments(const int new_segments)
{
    segments = new_segments;
}

double Cone3D::get_radius() const
{
    return radius;
}

double Cone3D::get_height() const
{
    return height;
}

int Cone3D::get_segments() const
{
    return segments;
}

}
