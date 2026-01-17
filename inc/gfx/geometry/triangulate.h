#pragma once

#include <vector>

#include "gfx/geometry/types/barycentric-triangle.h"
#include "gfx/geometry/types/polygon.h"
#include "gfx/math/vec2.h"

namespace gfx
{
    class Triangulate
    {

    public:

        static std::vector<BarycentricTriangle> triangulate_polygon(const Polygon::Component &component);

    private:

        static bool is_convex(const BarycentricTriangle &triangle, bool clockwise);
        static bool is_ear(
            int index,
            const std::vector<int> &indices,
            const std::vector<Vec2d> &vertices,
            const BarycentricTriangle &triangle,
            int i0,
            int i1,
            int i2,
            bool clockwise
        );
        static Polygon::Contour merge_holes(
            const Polygon::Contour &contour,
            const std::vector<Polygon::Contour> &holes
        );
    };
}
