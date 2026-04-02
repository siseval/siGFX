#pragma once

#include "gfx/core/primitive-3D.h"

namespace gfx
{
class Cone3D final : public Primitive3D
{

public:

    Cone3D() = default;

    const PolygonMesh &get_mesh() const override;

    void set_radius(const double new_radius);
    void set_height(const double new_height);
    void set_segments(const int new_segments);

    double get_radius() const;
    double get_height() const;
    int get_segments() const;

private:

    double radius;
    double height;
    int segments;
};
}
