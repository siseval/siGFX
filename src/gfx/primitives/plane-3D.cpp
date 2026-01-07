#include "gfx/primitives/plane-3D.h"

namespace gfx
{

const PolygonMesh& Plane3D::get_mesh() const
{
    if (!is_mesh_dirty())
    {
        return mesh_data;
    }

    mesh_data.clear();

    double w { size.x / 2 };
    double d { size.y / 2 };

    mesh_data.set_vertices({
        { Vec3d { -w, 0,  d }, Vec3d { 0, 1, 0 } },
        { Vec3d {  w, 0,  d }, Vec3d { 0, 1, 0 } },
        { Vec3d {  w, 0, -d }, Vec3d { 0, 1, 0 } },
        { Vec3d { -w, 0, -d }, Vec3d { 0, 1, 0 } }
    });

    mesh_data.set_triangles({
        { 0, 1, 2 }, 
        { 0, 2, 3 },

        { 0, 2, 1 }, 
        { 0, 3, 2 }
    });

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
