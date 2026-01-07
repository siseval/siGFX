#pragma once

#include "gfx/core/primitive-3D.h"

namespace gfx
{

class Polygon3D : public Primitive3D
{

public:

    const PolygonMesh& get_mesh() const override;

    void set_mesh(const PolygonMesh& mesh);
    void set_vertex_coords(const std::vector<Vec3d>& coords);
    void set_vertices(const std::vector<PolygonMesh::Vertex>& vertices);
    void set_triangles(const std::vector<PolygonMesh::Triangle>& triangles);

    void calculate_normals();

    void clear_mesh();
    
private:

};

}
