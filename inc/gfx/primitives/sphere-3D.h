#pragma once

#include "gfx/core/primitive-3D.h"

namespace gfx
{

class Sphere3D : public Primitive3D
{

public:

    const PolygonMesh& get_mesh() const override;

    void set_radius(const double new_radius);
    double get_radius() const;

private:

    double radius;

};

}
