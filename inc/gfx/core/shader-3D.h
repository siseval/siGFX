#pragma once

#include "gfx/core/types/color4.h"
#include "gfx/math/vec3.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Shader3D
{

public:

    struct VertInput
    {
        std::vector<Vec3d> positions;
        std::vector<Vec3d> normals;

        Matrix4x4d model_matrix;
        Matrix4x4d view_matrix;
        Matrix4x4d projection_matrix;
        Matrix4x4d mvp_matrix;
    };

    struct VertOutput
    {
        std::vector<Vec3d> xyz;
        std::vector<double> w;
        std::vector<Vec3d> normals;
    };

    struct FragInput
    {
        std::vector<Vec3d> uvw;
        std::vector<double> depths;
        std::vector<Vec3d> normals;
        std::vector<Color4> colors;

        double t;
        Vec3d light_dir;
        double ambient_intensity;
    };

    class VertShader
    {

    public:

        virtual VertOutput vert(const VertInput &input) const = 0;

    };

    class FragShader
    {

    public:

        virtual std::vector<Color4> frag(const FragInput &input) const = 0;

    };

    Shader3D(
        std::shared_ptr<VertShader> vertex_shader,
        std::shared_ptr<FragShader> fragment_shader
    ) : vertex_shader(vertex_shader), fragment_shader(fragment_shader) {}

    VertOutput vert(const VertInput &input) const
    {
        return vertex_shader->vert(input);
    }

    std::vector<Color4> frag(const FragInput &input) const
    {
        return fragment_shader->frag(input);
    }

    std::shared_ptr<VertShader> get_vertex_shader() const
    {
        return vertex_shader;
    }

    std::shared_ptr<FragShader> get_fragment_shader() const
    {
        return fragment_shader;
    }

protected:

    std::shared_ptr<VertShader> vertex_shader;
    std::shared_ptr<FragShader> fragment_shader;

};

}
