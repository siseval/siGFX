#pragma once

#include <cstdint>
#include <functional>

namespace gfx
{

struct Color4
{

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color4(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
    Color4(const int r, const int g, const int b, const int a);

    Color4(const uint8_t r, const uint8_t g, const uint8_t b);
    Color4(const int r, const int g, const int b);

    Color4(const double r, const double g, const double b, const double a);
    Color4(const double r, const double g, const double b);

    Color4(const double x);

    Color4();

    static Color4 from_i32(const int32_t color);

    static double distance(const Color4 &a, const Color4 &b);

    Color4 set_alpha(const uint8_t alpha) const;
    Color4 set_alpha(const double alpha) const;

    Color4 operator+(const Color4 &other) const;
    Color4 operator*(const double scalar) const;
    Color4 operator*(const Color4 &other) const;

    bool operator==(const Color4 &other) const;
    void operator=(const Color4 &other);

    float r_double() const;
    float g_double() const;
    float b_double() const;
    float a_double() const;

    int32_t to_i32() const;

    static Color4 lerp(const Color4 &a, const Color4 &b, double t);
    static Color4 trilinear_interp(const Color4 &c1, const Color4 &c2, const Color4 &c3, const double w1, const double w2, const double w3);
    
    const static Color4 black();
    const static Color4 white();
    const static Color4 red();
    const static Color4 green();
    const static Color4 blue();
};

}

template <>
struct std::hash<gfx::Color4>
{
    size_t operator()(const gfx::Color4& color) const
    {
        return std::hash<uint8_t>()(color.r ^ (std::hash<uint8_t>()(color.g) << 1)) ^ (std::hash<uint8_t>()(color.b) << 2);
    }
};
