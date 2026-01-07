#include "gfx/primitives/polygon-3D.h"

namespace gfx
{

const PolygonMesh& Polygon3D::get_mesh() const
{
    return mesh_data;
}

void Polygon3D::set_mesh(const PolygonMesh& mesh)
{
    mesh_data = mesh;
}

void Polygon3D::set_vertex_coords(const std::vector<Vec3d>& coords)
{
    mesh_data.clear();
    for (const auto& pos : coords)
    {
        mesh_data.add_vertex({ pos, Vec3d::zero() });
    }
}

void Polygon3D::set_vertices(const std::vector<PolygonMesh::Vertex>& vertices)
{
    mesh_data.set_vertices(vertices);
}

void Polygon3D::set_triangles(const std::vector<PolygonMesh::Triangle>& triangles)
{
    mesh_data.set_triangles(triangles);
}

void Polygon3D::calculate_normals()
{
    for (const auto& tri : mesh_data.get_triangles())
    {
        PolygonMesh::Vertex& v0 = mesh_data.get_vertices().at(tri.v0);
        PolygonMesh::Vertex& v1 = mesh_data.get_vertices().at(tri.v1);
        PolygonMesh::Vertex& v2 = mesh_data.get_vertices().at(tri.v2);

        Vec3d edge1 = v1.pos - v0.pos;
        Vec3d edge2 = v2.pos - v0.pos;

        Vec3d normal = Vec3d::cross(edge1, edge2).normalize();

        v0.normal = normal;
        v1.normal = normal;
        v2.normal = normal;
    }
}


}

