#include "gfx/primitives/sphere-3D.h"

namespace gfx
{


const PolygonMesh& Sphere3D::get_mesh() const
{
    if (!is_mesh_dirty())
    {
        return mesh_data;
    }

    mesh_data.clear();

    std::vector<Vec3d> vertices;
    std::vector<Vec3d> normals;
    std::vector<size_t> indices;

    for (int r = 0; r < lat_segments; ++r)
    {
        for (int s = 0; s < lon_segments; ++s)
        {
            const double phi0   = std::numbers::pi * r / lat_segments;
            const double phi1   = std::numbers::pi * (r + 1) / lat_segments;
            const double theta0 = 2.0 * std::numbers::pi * s / lon_segments;
            const double theta1 = 2.0 * std::numbers::pi * (s + 1) / lon_segments;

            auto get_pos = [&](const double phi, const double theta)
            {
                const double x = radius * std::sin(phi) * std::cos(theta);
                const double y = radius * std::cos(phi);
                const double z = radius * std::sin(phi) * std::sin(theta);

                Vec3d pos { x, y, z };
                Vec3d normal = pos.normalize();
                return std::make_pair(pos, normal);
            };

            auto v0 = get_pos(phi0, theta0);
            auto v1 = get_pos(phi0, theta1);
            auto v2 = get_pos(phi1, theta1);
            auto v3 = get_pos(phi1, theta0);

            auto emit = [&](const auto& a, const auto& b, const auto& c)
            {
                const size_t base = vertices.size();

                vertices.push_back(a.first);
                vertices.push_back(b.first);
                vertices.push_back(c.first);

                normals.push_back(a.second);
                normals.push_back(b.second);
                normals.push_back(c.second);

                indices.push_back(base + 0);
                indices.push_back(base + 1);
                indices.push_back(base + 2);
            };

            if (r == 0)
            {
                emit(v0, v2, v3);
            }
            else if (r == lat_segments - 1)
            {
                emit(v0, v1, v2);
            }
            else
            {
                emit(v0, v1, v2);
                emit(v0, v2, v3);
            }
        }
    }

    mesh_data.set_vertices(std::move(vertices));
    mesh_data.set_normals(std::move(normals));
    mesh_data.set_indices(std::move(indices));
    mesh_data.set_colors(std::vector(mesh_data.num_vertices(), get_color()));

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

void Sphere3D::set_num_lat_segments(const int segments)
{
    lat_segments = segments;
    set_mesh_dirty(true);
}

void Sphere3D::set_num_lon_segments(const int segments)
{
    lon_segments = segments;
    set_mesh_dirty(true);
}

void Sphere3D::set_num_segments(const int lat, const int lon)
{
    lat_segments = lat;
    lon_segments = lon;
    set_mesh_dirty(true);
}

void Sphere3D::set_num_segments(const int segments)
{
    lat_segments = segments;
    lon_segments = segments;
    set_mesh_dirty(true);
}

}
