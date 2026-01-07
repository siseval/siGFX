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

    void set_vertices(const std::vector<Vertex>& verts)
    {
        vertices = verts;
    }

    void set_triangles(const std::vector<Triangle>& tris)
    {
        indices = tris;
    }

    void add_vertex(const Vertex& vertex)
    {
        vertices.push_back(vertex);
    }

    void set_normal(const uint32_t vertex_index, const Vec3d& normal)
    {
        if (vertex_index < vertices.size())
        {
            vertices[vertex_index].normal = normal;
        }
    }

    std::vector<Vertex>& get_vertices()
    {
        return vertices;
    }

    std::vector<Triangle>& get_triangles()
    {
        return indices;
    }

    void clear()
    {
        vertices.clear();
        indices.clear();
    }

private:

    std::vector<Vertex> vertices;
    std::vector<Triangle> indices;

};

}
