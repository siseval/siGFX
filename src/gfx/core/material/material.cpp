#include "gfx/core/material/material.h"

namespace gfx
{

Material::Material()
    : id(next_id()) {}

Material::Material(const std::shared_ptr<VertexShader> &vert_shader, const std::shared_ptr<FragmentShader> &frag_shader)
    : vertex_shader(vert_shader),
      fragment_shader(frag_shader),
      id(next_id()) {}

Material::Material(const std::shared_ptr<VertexShader> &vert_shader, const std::shared_ptr<FragmentShader> &frag_shader, const std::shared_ptr<Texture> &tex)
    : vertex_shader(vert_shader),
      fragment_shader(frag_shader),
      texture(tex),
      id(next_id()) {}

int Material::next_id()
{
    static int next_id = 0;
    return next_id++;
}

int Material::get_id() const
{
    return id;
}

void Material::set_texture(const std::shared_ptr<Texture> &tex)
{
    texture = tex;
}

void Material::set_vertex_shader(const std::shared_ptr<VertexShader> &shader)
{
    vertex_shader = shader;
}

void Material::set_fragment_shader(const std::shared_ptr<FragmentShader> &shader)
{
    fragment_shader = shader;
}

std::shared_ptr<Texture> Material::get_texture() const
{
    return texture;
}

std::shared_ptr<VertexShader> Material::get_vertex_shader() const
{
    return vertex_shader;
}

std::shared_ptr<FragmentShader> Material::get_fragment_shader() const
{
    return fragment_shader;
}

}
