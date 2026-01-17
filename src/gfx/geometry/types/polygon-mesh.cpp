#include "gfx/geometry/types/polygon-mesh.h"

namespace gfx
{

    PolygonMesh::PolygonMesh() {}

    PolygonMesh::PolygonMesh(std::vector<Vec3d> vertices, std::vector<Vec3d> normals, std::vector<size_t> indices)
        : vertices(vertices),
          normals(normals),
          indices(indices),
          colors(std::vector(vertices.size(), Color4::white())) {}

    Box3d PolygonMesh::get_extent() const
    {
        if (vertices.empty())
        {
            return Box3d { Vec3d { 0, 0, 0 }, Vec3d { 0, 0, 0 } };
        }

        Box3d extent { vertices[0], vertices[0] };

        for (const auto &vertex : vertices)
        {
            extent.expand(vertex);
        }

        return extent;
    }

    void PolygonMesh::set_vertices(const std::vector<Vec3d> verts)
    {
        vertices = verts;
    }

    void PolygonMesh::set_normals(const std::vector<Vec3d> norms)
    {
        normals = norms;
    }

    void PolygonMesh::set_indices(const std::vector<size_t> inds)
    {
        indices = inds;
    }

    void PolygonMesh::set_colors(const std::vector<Color4> cols)
    {
        colors = cols;
    }

    const std::vector<Vec3d> &PolygonMesh::get_vertices() const
    {
        return vertices;
    }

    const std::vector<Vec3d> &PolygonMesh::get_normals() const
    {
        return normals;
    }

    const std::vector<size_t> &PolygonMesh::get_indices() const
    {
        return indices;
    }

    const std::vector<Color4> &PolygonMesh::get_colors() const
    {
        return colors;
    }

    size_t PolygonMesh::num_vertices() const
    {
        return vertices.size();
    }

    void PolygonMesh::clear()
    {
        vertices.clear();
        normals.clear();
        indices.clear();
        colors.clear();
    }

}
