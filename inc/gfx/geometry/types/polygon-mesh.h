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

    PolygonMesh();

    PolygonMesh(std::vector<Vec3d> vertices, std::vector<Vec3d> normals, std::vector<size_t> indices);

    Box3d get_extent() const;

    void set_vertices(const std::vector<Vec3d> verts);
    void set_normals(const std::vector<Vec3d> norms);
    void set_indices(const std::vector<size_t> inds);
    void set_colors(const std::vector<Color4> cols);

    const std::vector<Vec3d>& get_vertices() const;
    const std::vector<Vec3d>& get_normals() const;
    const std::vector<size_t>& get_indices() const;
    const std::vector<Color4>& get_colors() const;

    const size_t num_vertices() const;

    void clear();

private:

    std::vector<Vec3d> vertices;
    std::vector<Vec3d> normals;
    std::vector<size_t> indices;
    std::vector<Color4> colors;

};

}
