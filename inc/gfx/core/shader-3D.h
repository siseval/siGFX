#pragma once

#include "gfx/math/vec3.h"
#include "gfx/core/types/color4.h"
#include "gfx/math/matrix.h"

namespace gfx
{

class Shader3D
{

public:

    struct VertInput
    {
        Vec3d pos;
        Vec3d normal;
    };

    struct VertOutput
    {
        Vec3d xyz;
        double w;
        Vec3d normal;
    };

    struct FragInput
    {
        Vec3d uvw;
        double depth;
        double t;
        Vec3d normal;
    };

    class VertShader
    {

    public:

        virtual VertOutput vert(const VertInput &input) const = 0;

        void set_matrices(
            const Matrix4x4d& model,
            const Matrix4x4d& view,
            const Matrix4x4d& projection)
        {
            model_matrix = model;
            view_matrix = view;
            projection_matrix = projection;
            mvp_matrix = projection_matrix * view_matrix * model_matrix;
        }

    protected:

        Matrix4x4d model_matrix;
        Matrix4x4d view_matrix;
        Matrix4x4d projection_matrix;
        Matrix4x4d mvp_matrix;
    };

    class FragShader
    {

    public:

        virtual Color4 frag(const FragInput &input) const = 0;

        void set_light_direction(const Vec3d& dir)
        {
            light_dir = dir.normalize();
        }

        void set_ambient_intensity(const double intensity)
        {
            ambient_intensity = intensity;
        }

    protected:

        Vec3d light_dir;
        double ambient_intensity;

    };

    VertOutput vert(const VertInput &input) const
    {
        return vertex_shader->vert(input);
    }

    Color4 frag(const FragInput &input) const
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
