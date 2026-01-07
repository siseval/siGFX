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

    void set_num_lat_segments(const int segments);
    void set_num_lon_segments(const int segments);
    void set_num_segments(const int lat, const int lon);
    void set_num_segments(const int segments);

    int get_lat_segments() const;

private:

    double radius;
    int lat_segments = 12;
    int lon_segments = 12;

};

}
