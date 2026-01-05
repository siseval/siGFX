#include "gfx/geometry/polygon-mesh.h"

namespace gfx
{

Box3d PolygonMesh::get_extent() const
{
    if (vertices.empty())
    {
        return Box3d { Vec3d { 0, 0, 0 }, Vec3d { 0, 0, 0 } };
    }

    Box3d extent { vertices[0].pos, vertices[0].pos };

    for (const auto& vertex : vertices)
    {
        extent.expand(vertex.pos);
    }

    return extent;
}

}
