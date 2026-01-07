#include "gfx/primitives/sphere-3D.h"

namespace gfx
{

const PolygonMesh& Sphere3D::get_mesh() const
{
    if (!is_mesh_dirty())
    {
        return mesh_data;
    }

    int rings = 12;
    int segments = 12;

    std::vector<PolygonMesh::Vertex> sphere_vertices;

    for (int r = 0; r < rings; ++r) 
    {
        for (int s = 0; s < segments; ++s) 
        {
            double phi0 = std::numbers::pi * r / rings;
            double phi1 = std::numbers::pi * (r + 1) / rings;
            double theta0 = 2.0 * std::numbers::pi * s / segments;
            double theta1 = 2.0 * std::numbers::pi * (s + 1) / segments;

            auto get_pos = [&](double phi, double theta) {
                double x = radius * std::sin(phi) * std::cos(theta);
                double y = radius * std::cos(phi);
                double z = radius * std::sin(phi) * std::sin(theta);

                Vec3d pos { x, y, z };
                Vec3d normal = pos.normalize();
                return std::make_pair(pos, normal);
            };

            auto v0 = get_pos(phi0, theta0);
            auto v1 = get_pos(phi0, theta1);
            auto v2 = get_pos(phi1, theta1);
            auto v3 = get_pos(phi1, theta0);

            if (r == 0) 
            {
                sphere_vertices.push_back({ v0.first, v0.second });
                sphere_vertices.push_back({ v2.first, v2.second });
                sphere_vertices.push_back({ v3.first, v3.second });
            }

            else if (r == rings - 1) 
            {
                sphere_vertices.push_back({ v0.first, v0.second });
                sphere_vertices.push_back({ v1.first, v1.second });
                sphere_vertices.push_back({ v2.first, v2.second });
            }

            else
            {
                sphere_vertices.push_back({ v0.first, v0.second });
                sphere_vertices.push_back({ v1.first, v1.second });
                sphere_vertices.push_back({ v2.first, v2.second });

                sphere_vertices.push_back({ v0.first, v0.second });
                sphere_vertices.push_back({ v2.first, v2.second });
                sphere_vertices.push_back({ v3.first, v3.second });
            }
        }
    }

    mesh_data.set_vertices(sphere_vertices);

    std::vector<PolygonMesh::Triangle> sphere_triangles;
    for (uint32_t i = 0; i < sphere_vertices.size(); i += 3) 
    {
        sphere_triangles.push_back({ i, i + 1, i + 2 });
    }

    mesh_data.set_triangles(sphere_triangles);

    set_mesh_dirty(false);

    return mesh_data;
}

void Sphere3D::set_radius(const double r)
{
    radius = r;
    set_mesh_dirty(true);
}

double Sphere3D::get_radius() const
{
    return radius;
}

}
