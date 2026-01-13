#pragma once

#include "gfx/math/vec3.h"
#include "gfx/math/box3.h"
#include "gfx/core/types/color4.h"

#include <vector>

namespace gfx
{

class PolygonMesh
{

public:

    PolygonMesh() = default;

    PolygonMesh(std::vector<Vec3d> vertices, std::vector<Vec3d> normals, std::vector<size_t> indices) : 
        vertices(vertices), 
        normals(normals), 
        indices(indices), 
        colors(std::vector<Color4>(vertices.size(), Color4::white())) {}

    Box3d get_extent() const; 

    void set_vertices(const std::vector<Vec3d> verts)
    {
        vertices = verts;
    }

    void set_normals(const std::vector<Vec3d> norms)
    {
        normals = norms;
    }

    void set_indices(const std::vector<size_t> inds)
    {
        indices = inds;
    }

    void set_colors(const std::vector<Color4> cols)
    {
        colors = cols;
    }

    const std::vector<Vec3d>& get_vertices() const
    {
        return vertices;
    }

    const std::vector<Vec3d>& get_normals() const
    {
        return normals;
    }

    const std::vector<size_t>& get_indices() const
    {
        return indices;
    }

    const std::vector<Color4>& get_colors() const
    {
        return colors;
    }

    const size_t num_vertices() const
    {
        return vertices.size();
    }

    void clear()
    {
        vertices.clear();
        normals.clear();
        indices.clear();
        colors.clear();
    }

private:

    std::vector<Vec3d> vertices;
    std::vector<Vec3d> normals;
    std::vector<size_t> indices;
    std::vector<Color4> colors;

};

}
