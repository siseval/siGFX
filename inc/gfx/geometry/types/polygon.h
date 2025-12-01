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
    Contour(std::vector<Vec2d> verts, bool cw)
        : vertices(verts), clockwise(cw) {}

    Contour() : vertices(), clockwise(true) {}

    std::vector<Vec2d> vertices;
    bool clockwise;
};

struct Component
{
    Component(std::vector<Vec2d> vertices, bool clockwise)
        : contour(vertices, clockwise) {}

    Component(Contour cont)
        : contour(cont) {}

    Component(std::vector<Vec2d> vertices, bool cw, std::vector<Contour> holes)
        : contour(vertices, cw), holes(holes) {}

    Component() : contour(), holes() {}

    Contour contour;
    std::vector<Contour> holes;
};

};

}
