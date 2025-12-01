#pragma once

#include <numbers>
#include <cmath>
#include <functional>

namespace gfx
{

template <typename T>
class Vec3 
{
public:

    T x;
    T y;
    T z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    Vec3(const Vec3<T> &other) : x(other.x), y(other.y), z(other.z) {}
    Vec3(T v) : x(v), y(v), z(v) {}

    static Vec3<T> zero() 
    { 
        return { 0, 0, 0 }; 
    }

    static T dot(Vec3<T> a, Vec3<T> b)
    {
        return static_cast<T>(a.x * b.x + a.y * b.y + a.z * b.z);
    }

    static Vec3<T> cross(Vec3<T> a, Vec3<T> b)
    {
        return {
            static_cast<T>(a.y * b.z - a.z * b.y),
            static_cast<T>(a.z * b.x - a.x * b.z),
            static_cast<T>(a.x * b.y - a.y * b.x)
        };
    }

    static double distance(Vec3<T> a, Vec3<T> b)
    {
        return (b - a).length();
    }

    static Vec3<T> lerp(const Vec3<T> &a, const Vec3<T> &b, double t)
    {
        t = t < 0 ? 0 : (t > 1 ? 1 : t);
        return { 
            static_cast<T>(a.x + (b.x - a.x) * t), 
            static_cast<T>(a.y + (b.y - a.y) * t), 
            static_cast<T>(a.z + (b.z - a.z) * t) 
        };
    }

    static Vec3<T> from_angles(double theta, double phi, double length = 1.0)
    {
        double sin_phi = std::sin(phi);
        return {
            static_cast<T>(length * sin_phi * std::cos(theta)),
            static_cast<T>(length * sin_phi * std::sin(theta)),
            static_cast<T>(length * std::cos(phi))
        };
    }

    static Vec3<T> from_angles_degrees(double theta_deg, double phi_deg, double length = 1.0)
    {
        return from_angles(theta_deg * std::numbers::pi / 180.0,
                           phi_deg * std::numbers::pi / 180.0,
                           length);
    }

    inline Vec3<T> limit(const double max_magnitude)
    {
        double magnitude = length();
        if (magnitude > max_magnitude)
        {
            return normalize() * max_magnitude;
        }
        return { x, y, z };
    }

    Vec3<double> normal() const
    {
        if (std::fabs(x) > 1e-6 || std::fabs(y) > 1e-6)
            return Vec3<double>{-y, x, 0}.normalize();
        return Vec3<double>{0, -z, y}.normalize();
    }

    double length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3<double> normalize() const
    {
        return { static_cast<T>(x / length()), static_cast<T>(y / length()), static_cast<T>(z / length()) };
    }

    Vec3<T> operator-() const
    {
        return { static_cast<T>(-x), static_cast<T>(-y), static_cast<T>(-z) };
    }

    template <typename U>
    Vec3<T> operator+(const Vec3<U> &other) const
    {
        return { static_cast<T>(x + other.x), static_cast<T>(y + other.y), static_cast<T>(z + other.z) };
    }

    template <typename U>
    Vec3<T> operator-(const Vec3<U> &other) const
    {
        return { static_cast<T>(x - other.x), static_cast<T>(y - other.y), static_cast<T>(z - other.z) };
    }

    template <typename U>
    Vec3<T> operator*(const Vec3<U> &other) const
    {
        return { static_cast<T>(x * other.x), static_cast<T>(y * other.y), static_cast<T>(z * other.z) };
    }

    template <typename U>
    Vec3<T> operator/(const Vec3<U> &other) const
    {
        return { static_cast<T>(x / other.x), static_cast<T>(y / other.y), static_cast<T>(z / other.z) };
    }

    Vec3<T> operator*(const double &factor) const
    {
        return { static_cast<T>(x * factor), static_cast<T>(y * factor), static_cast<T>(z * factor) };
    }

    Vec3<T> operator/(const double &factor) const
    {
        return { static_cast<T>(x / factor), static_cast<T>(y / factor), static_cast<T>(z / factor) };
    }

    template <typename U>
    Vec3<T>& operator+=(const Vec3<U> &other)
    {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    template <typename U>
    Vec3<T>& operator-=(const Vec3<U> &other)
    {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    template <typename U>
    Vec3<T>& operator*=(const Vec3<U> &other)
    {
        x *= other.x; y *= other.y; z *= other.z;
        return *this;
    }

    template <typename U>
    Vec3<T>& operator/=(const Vec3<U> &other)
    {
        x /= other.x; y /= other.y; z /= other.z;
        return *this;
    }

    Vec3<T>& operator*=(const double &factor)
    {
        x *= factor; y *= factor; z *= factor;
        return *this;
    }

    Vec3<T>& operator/=(const double &factor)
    {
        x /= factor; y /= factor; z /= factor;
        return *this;
    }

    bool operator==(const Vec3<T> &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vec3<T> &other) const
    {
        return x != other.x || y != other.y || z != other.z;
    }

    void operator=(const Vec3<T> &other)
    {
        x = other.x; y = other.y; z = other.z;
    }

    template <typename U>
    operator Vec3<U>() const
    {
        return { static_cast<U>(x), static_cast<U>(y), static_cast<U>(z) };
    }
};

using Vec3i = Vec3<int>;
using Vec3d = Vec3<double>;
using Vec3f = Vec3<float>;

}

template <typename T>
struct std::hash<gfx::Vec3<T>>
{
    size_t operator()(const gfx::Vec3<T>& vec) const
    {
        return std::hash<T>()(vec.x) ^ (std::hash<T>()(vec.y) << 1) ^ (std::hash<T>()(vec.z) << 2);
    }
};
