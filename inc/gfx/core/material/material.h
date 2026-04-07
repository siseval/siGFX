#pragma once

#include "gfx/core/material/texture.h"
#include "gfx/core/material/vertex-shader.h"
#include "gfx/core/material/fragment-shader.h"

namespace gfx
{

class Material
{

public:

    Material();
    Material(const std::shared_ptr<VertexShader> &vert_shader, const std::shared_ptr<FragmentShader> &frag_shader);
    Material(const std::shared_ptr<VertexShader> &vert_shader, const std::shared_ptr<FragmentShader> &frag_shader, const std::shared_ptr<Texture> &tex);

    int get_id() const;

    void set_texture(const std::shared_ptr<Texture> &tex);
    void set_vertex_shader(const std::shared_ptr<VertexShader> &shader);
    void set_fragment_shader(const std::shared_ptr<FragmentShader> &shader);

    std::shared_ptr<Texture> get_texture() const;
    std::shared_ptr<VertexShader> get_vertex_shader() const;
    std::shared_ptr<FragmentShader> get_fragment_shader() const;

    virtual ~Material() = default;

private:

    static int next_id();

    std::shared_ptr<Texture> texture;
    std::shared_ptr<VertexShader> vertex_shader;
    std::shared_ptr<FragmentShader> fragment_shader;

    int id;

};
}
