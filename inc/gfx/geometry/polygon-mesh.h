#pragma once

#include "gfx/math/vec3.h"
#include "gfx/math/box3.h"

#include <vector>
#include <cstdint>

namespace gfx
{

class PolygonMesh
{

public:

    struct Vertex
    {
        gfx::Vec3d pos;
        Vec3d normal;
    };

    struct Triangle
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
    };

    PolygonMesh() = default;

    PolygonMesh(const std::vector<Vertex>& verts, const std::vector<Triangle>& tris)
        : vertices(verts), indices(tris) {}

    Box3d get_extent() const; 

    void add_vertex(const Vertex& vertex)
    {
        vertices.push_back(vertex);
    }

    const std::vector<Vertex>& get_vertices() const
    {
        return vertices;
    }

    const std::vector<Triangle>& get_triangles() const
    {
        return indices;
    }

private:

    std::vector<Vertex> vertices;
    std::vector<Triangle> indices;

};

}
