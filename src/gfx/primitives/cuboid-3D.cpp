#include "gfx/primitives/cuboid-3D.h"

namespace gfx
{


const PolygonMesh& Cuboid3D::get_mesh() const
{
    if (!is_mesh_dirty())
    {
        return mesh_data;
    }

    mesh_data.clear();

    double w = size.x * 0.5;
    double h = size.y * 0.5;
    double d = size.z * 0.5;

    std::vector<Vec3d> vertices = {
        { -w, -h, -d }, { -w,  h, -d }, {  w,  h, -d },
        { -w, -h, -d }, {  w,  h, -d }, {  w, -h, -d },

        { -w, -h,  d }, {  w, -h,  d }, {  w,  h,  d },
        { -w, -h,  d }, {  w,  h,  d }, { -w,  h,  d },

        { -w, -h,  d }, { -w,  h,  d }, { -w,  h, -d },
        { -w, -h,  d }, { -w,  h, -d }, { -w, -h, -d },

        {  w, -h, -d }, {  w,  h, -d }, {  w,  h,  d },
        {  w, -h, -d }, {  w,  h,  d }, {  w, -h,  d },

        { -w,  h, -d }, { -w,  h,  d }, {  w,  h,  d },
        { -w,  h, -d }, {  w,  h,  d }, {  w,  h, -d },

        { -w, -h,  d }, { -w, -h, -d }, {  w, -h, -d },
        { -w, -h,  d }, {  w, -h, -d }, {  w, -h,  d }
    };

    std::vector<Vec3d> normals = {
        {  0,  0, -1 }, {  0,  0, -1 }, {  0,  0, -1 },
        {  0,  0, -1 }, {  0,  0, -1 }, {  0,  0, -1 },

        {  0,  0,  1 }, {  0,  0,  1 }, {  0,  0,  1 },
        {  0,  0,  1 }, {  0,  0,  1 }, {  0,  0,  1 },

        { -1,  0,  0 }, { -1,  0,  0 }, { -1,  0,  0 },
        { -1,  0,  0 }, { -1,  0,  0 }, { -1,  0,  0 },

        {  1,  0,  0 }, {  1,  0,  0 }, {  1,  0,  0 },
        {  1,  0,  0 }, {  1,  0,  0 }, {  1,  0,  0 },

        {  0,  1,  0 }, {  0,  1,  0 }, {  0,  1,  0 },
        {  0,  1,  0 }, {  0,  1,  0 }, {  0,  1,  0 },

        {  0, -1,  0 }, {  0, -1,  0 }, {  0, -1,  0 },
        {  0, -1,  0 }, {  0, -1,  0 }, {  0, -1,  0 }
    };

    std::vector<size_t> indices = {
         0,  1,  2,  3,  4,  5,
         6,  7,  8,  9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
    };

    mesh_data.set_vertices(std::move(vertices));
    mesh_data.set_normals(std::move(normals));
    mesh_data.set_indices(std::move(indices));
    mesh_data.set_colors(std::vector(mesh_data.num_vertices(), get_color()));

    set_mesh_dirty(false);

    return mesh_data;
}


void Cuboid3D::set_size(const Vec3d& new_size)
{
    size = new_size;
    set_mesh_dirty(true);
}

void Cuboid3D::set_size(const double width, const double height, const double depth)
{
    size = Vec3d { width, height, depth };
    set_mesh_dirty(true);
}

Vec3d Cuboid3D::get_size() const
{
    return size;
}

}
