#ifndef SHADER_2D_H
#define SHADER_2D_H

#include <gfx/core/types/color4.h>
#include <gfx/math/vec2.h>

namespace gfx::core
{

struct ShaderInput2D
{
    gfx::math::Vec2d uv;
    double t;
};

constexpr std::size_t max_shader_size = 256;

struct ShaderHandle 
{
    using InvokeFn = types::Color4(*)(const void*, const ShaderInput2D&);
    using DestroyFn = void(*)(void*);

    alignas(std::max_align_t) unsigned char storage[max_shader_size];
    InvokeFn  invoke = nullptr;
    DestroyFn destroy = nullptr;

    ShaderHandle() = default;

    ShaderHandle(const ShaderHandle& other) 
    {
        if (!other.invoke) 
        {
            return;
        }
        std::memcpy(storage, other.storage, max_shader_size);
        invoke  = other.invoke;
        destroy = other.destroy;
    }

    ShaderHandle& operator=(const ShaderHandle& other) {
        if (this == &other) 
        {
            return *this;
        }
        if (destroy) 
        {
            destroy(storage);
        }
        if (!other.invoke) 
        {
            invoke = nullptr;
            destroy = nullptr;
            return *this;
        }
        std::memcpy(storage, other.storage, max_shader_size);
        invoke  = other.invoke;
        destroy = other.destroy;
        return *this;
    }

    ShaderHandle(ShaderHandle&& other) 
    {
        std::memcpy(storage, other.storage, max_shader_size);
        invoke  = other.invoke;
        destroy = other.destroy;
        other.invoke = nullptr;
        other.destroy = nullptr;
    }

    ShaderHandle& operator=(ShaderHandle&& other)
    {
        if (this == &other) 
        {
            return *this;
        }
        if (destroy) 
        {
            destroy(storage);
        }
        std::memcpy(storage, other.storage, max_shader_size);
        invoke  = other.invoke;
        destroy = other.destroy;
        other.invoke = nullptr;
        other.destroy = nullptr;
        return *this;
    }

    ~ShaderHandle() 
    {
        if (destroy) 
        {
            destroy(storage);
        }
    }

    types::Color4 operator()(const ShaderInput2D &input) const 
    {
        return invoke(storage, input);
    }

    template<class Shader>
    static ShaderHandle make(const Shader& shader) 
    {
        static_assert(sizeof(Shader) <= max_shader_size,
                      "Shader too large for inline storage");

        ShaderHandle handle;
        new (handle.storage) Shader(shader);

        handle.invoke = [](const void* obj, const ShaderInput2D & input) -> types::Color4 {
            const Shader* shader = static_cast<const Shader*>(obj);
            return shader->frag(input);
        };

        handle.destroy = [](void* obj) {
            static_cast<Shader*>(obj)->~Shader();
        };

        return handle;
    }
};

template<class Derived>
class Shader2D
{

public:

    types::Color4 frag(const ShaderInput2D &input)
    {
        return static_cast<Derived*>(this)->frag(input);
    }

    static inline types::Color4 mix(const types::Color4 &a, const types::Color4 &b, const double factor)
    {
        double clamped_factor = std::clamp(factor, 0.0, 1.0);
        return types::Color4(
            static_cast<uint8_t>(a.r * (1.0 - clamped_factor) + b.r * clamped_factor),
            static_cast<uint8_t>(a.g * (1.0 - clamped_factor) + b.g * clamped_factor),
            static_cast<uint8_t>(a.b * (1.0 - clamped_factor) + b.b * clamped_factor),
            static_cast<uint8_t>(a.a * (1.0 - clamped_factor) + b.a * clamped_factor)
        );
    }


private:



};

}

#endif // SHADER_2D_H
