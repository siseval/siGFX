#pragma once

#include <vector>

#include "gfx/math/vec2.h"

namespace gfx
{

class Flatten
{

public:

    static std::vector<Vec2d> flatten_bezier(const Vec2d p0, const Vec2d p1, const Vec2d p2, const double tolerance = 0.5);
    static std::vector<Vec2d> flatten_contour(const std::vector<std::pair<Vec2d, bool>> &points_on_off_curve);

};

}
