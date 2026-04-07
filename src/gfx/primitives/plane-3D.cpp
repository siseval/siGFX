#include "gfx/primitives/plane-3D.h"

namespace gfx
{


const PolygonMesh &Plane3D::get_mesh() const
{
    if (!is_mesh_dirty())
    {
        return mesh_data;
    }

    mesh_data.clear();

    const double w { size.x * 0.5 };
    const double d { size.y * 0.5 };

    const std::vector<Vec3d> vertices = {
        { -w, 0, d },
        { w, 0, d },
        { w, 0, -d },
        { -w, 0, -d }
    };

    const std::vector<Vec3d> normals = {
        { 0, 1, 0 },
        { 0, 1, 0 },
        { 0, 1, 0 },
        { 0, 1, 0 }
    };

    const std::vector<Vec2d> uvs = {
        { 0.0, 1.0 },
        { 1.0, 1.0 },
        { 1.0, 0.0 },
        { 0.0, 0.0 }
    };

    const std::vector<PolygonMesh::Face> faces = {
        { 0, 1, 2 },
        { 0, 2, 3 },

        { 0, 2, 1 },
        { 0, 3, 2 }
    };

    mesh_data.set_vertices(std::move(vertices));
    mesh_data.set_normals(std::move(normals));
    mesh_data.set_uvs(std::move(uvs));
    mesh_data.set_faces(std::move(faces));
    mesh_data.set_colors(std::vector(4, get_color()));

    set_mesh_dirty(false);

    return mesh_data;
}


void Plane3D::set_size(const Vec2d new_size)
{
    size = new_size;
}

void Plane3D::set_size(const double width, const double height)
{
    size = Vec2d { width, height };
}

Vec2d Plane3D::get_size() const
{
    return size;
}


}
