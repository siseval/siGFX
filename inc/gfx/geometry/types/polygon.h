#pragma once

#include <vector>

#include "gfx/math/vec2.h"

namespace gfx
{
    class Polygon
    {

    public:

        struct Contour
        {
            Contour(std::vector<Vec2d> verts, const bool cw)
                : vertices(verts), clockwise(cw) {}

            Contour() : clockwise(true) {}

            std::vector<Vec2d> vertices;
            bool clockwise;
        };

        struct Component
        {
            Component(std::vector<Vec2d> vertices, const bool clockwise)
                : contour(vertices, clockwise) {}

            explicit Component(Contour cont)
                : contour(cont) {}

            Component(std::vector<Vec2d> vertices, const bool cw, std::vector<Contour> holes)
                : contour(vertices, cw), holes(holes) {}

            Component() {}

            Contour contour;
            std::vector<Contour> holes;
        };
    };
}
