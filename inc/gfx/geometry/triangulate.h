#pragma once

#include <vector>

#include "gfx/math/vec2.h"
#include "gfx/geometry/types/barycentric-triangle.h"
#include "gfx/geometry/types/polygon.h"

namespace gfx
{

class Triangulate
{

public:

    static std::vector<BarycentricTriangle> triangulate_polygon(const Polygon::Component &component);

private:

    static bool is_convex(const BarycentricTriangle &triangle, const bool clockwise);
    static bool is_ear(const int index, const std::vector<int> &indices, const std::vector<Vec2d> &vertices, const BarycentricTriangle &triangle, const int i0, const int i1, const int i2, const bool clockwise);
    static Polygon::Contour merge_holes(const Polygon::Contour &contour, const std::vector<Polygon::Contour> &holes);

};

}
