#pragma once

#include "gfx/core/types/color4.h"
#include "gfx/math/matrix.h"
#include "gfx/math/vec3.h"

namespace gfx
{
class Shader3D
{
    static int next_id()
    {
        static int next_id = 0;
        return next_id++;
    }

    int id;

public:

    Shader3D() : id(next_id()) {}

    int get_id() const
    {
        return id;
    }

    struct VertUniforms
    {
        Matrix4x4d model_matrix;
        Matrix4x4d view_matrix;
        Matrix4x4d projection_matrix;
        Matrix4x4d mvp_matrix;
    };

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

    struct FragUniforms
    {
        double t;
        Vec3d light_dir;
        double ambient_intensity;
    };

    struct FragInput
    {
        Vec3d uvw;
        double depth;
        Vec3d normal;
        Color4 color;
    };

    class VertShader
    {

    public:

        virtual ~VertShader() = default;

        virtual VertOutput vert(const VertInput &input) const = 0;

        void set_uniforms(const VertUniforms &uni)
        {
            uniforms = uni;
        }

    protected:

        VertUniforms uniforms;
    };

    class FragShader
    {

    public:

        virtual ~FragShader() = default;

        virtual Color4 frag(const FragInput &input) const = 0;

        void set_uniforms(const FragUniforms &uni)
        {
            uniforms = uni;
        }

    protected:

        FragUniforms uniforms;
    };

    Shader3D(
        std::shared_ptr<VertShader> vertex_shader,
        std::shared_ptr<FragShader> fragment_shader
    ) : id(next_id()), vertex_shader(vertex_shader), fragment_shader(fragment_shader) {}

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
