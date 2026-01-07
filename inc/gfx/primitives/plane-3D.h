#pragma once

#include "gfx/core/primitive-3D.h"
#include "gfx/math/vec2.h"

namespace gfx
{

class Plane3D : public Primitive3D
{

public:

    Plane3D() = default;

    const PolygonMesh& get_mesh() const override;

    void set_size(const Vec2d new_size);
    void set_size(const double width, const double height);
    Vec2d get_size() const;

private:

    Vec2d size;

};

}
