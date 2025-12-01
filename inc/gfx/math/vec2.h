#pragma once

#include <numbers>
#include <cstdint>
#include <cmath>
#include <functional>

namespace gfx
{

template <typename T>
class Vec2 
{

public:

    T x;
    T y;

    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}
    Vec2(const Vec2<T> &other) : x(other.x), y(other.y) {}
    Vec2(T v) : x(v), y(v) {}

    static Vec2<T> zero() 
    { 
        return { 0, 0 }; 
    }

    static T cross(Vec2<T> a, Vec2<T> b) 
    { 
        return static_cast<T>(a.x * b.y - a.y * b.x); 
    }

    static T dot(Vec2<T> a, Vec2<T> b)
    {
        return static_cast<T>(a.x * b.x + a.y * b.y);
    }

    static double distance(Vec2<T> a, Vec2<T> b) 
    { 
        return (b - a).length();
    }

    static Vec2<T> from_angle(double angle, double length = 1.0) 
    { 
        return { static_cast<T>(std::cos(angle) * length), static_cast<T>(std::sin(angle) * length) }; 
    }

    static Vec2<T> from_angle_degrees(double angle, double length = 1.0) 
    { 
        return from_angle(angle * std::numbers::pi / 180.0, length); 
    }

    static Vec2<T> bezier(const Vec2<T> &p0, const Vec2<T> &p1, const Vec2<T> &p2, double t) 
    { 
        t = t < 0 ? 0 : (t > 1 ? 1 : t);
        double u = 1 - t;
        return { 
            static_cast<T>(u * u * p0.x + 2 * u * t * p1.x + t * t * p2.x), 
            static_cast<T>(u * u * p0.y + 2 * u * t * p1.y + t * t * p2.y) 
        }; 
    }

    static Vec2<T> lerp(const Vec2<T> &a, const Vec2<T> &b, double t) 
    { 
        t = t < 0 ? 0 : (t > 1 ? 1 : t);
        return { static_cast<T>(a.x + (b.x - a.x) * t), static_cast<T>(a.y + (b.y - a.y) * t) }; 
    }

    inline Vec2<T> limit(const double max_magnitude)
    {
        double magnitude = length();
        if (magnitude > max_magnitude)
        {
            return normalize() * max_magnitude;
        }
        return { x, y };
    }

    Vec2<T> rotate_towards(Vec2<T> &target, double max_angle) const
    { 
        double current_angle { angle() };
        double target_angle { target.angle() };
        double angle_diff { target_angle - current_angle };

        if (angle_diff > std::numbers::pi) 
        {
            angle_diff -= 2 * std::numbers::pi;
        }
        if (angle_diff < -std::numbers::pi) 
        {
            angle_diff += 2 * std::numbers::pi;
        }

        if (std::fabs(angle_diff) <= max_angle) 
        {
            return target;
        } 
        else 
        {
            double new_angle { current_angle + (angle_diff > 0 ? max_angle : -max_angle) };
            return from_angle(new_angle, length());
        }
    }

    Vec2<T> rotate_towards_degrees(Vec2<T> &target, double max_angle) const
    { 
        return rotate_towards(target, max_angle * std::numbers::pi / 180.0); 
    }

    double angle() const
    { 
        return std::atan2(y, x);
    }

    double angle_degrees() const
    { 
        return angle() * 180.0 / std::numbers::pi;
    }

    Vec2<double> normalize() const
    { 
        return { static_cast<T>(x / length()), static_cast<T>(y / length()) }; 
    }

    Vec2<double> normal() const 
    { 
        return { -y, x }; 
    }

    double length() const
    { 
        return std::sqrt(x * x + y * y); 
    }

    Vec2<int> round() const 
    { 
        return { static_cast<int>(std::lround(x)), static_cast<int>(std::lround(y)) }; 
    }

    static Vec2<int> floor(const Vec2<T> &v) 
    { 
        return { static_cast<int>(std::floor(v.x)), static_cast<int>(std::floor(v.y)) }; 
    }

    static Vec2<int> ceil(const Vec2<T> &v) 
    { 
        return { static_cast<int>(std::ceil(v.x)), static_cast<int>(std::ceil(v.y)) }; 
    }
    

    template <typename U>
    Vec2<T> operator+(const Vec2<U> &other) const 
    { 
        return { static_cast<T>(x + other.x), static_cast<T>(y + other.y) }; 
    }
    template <typename U>
    Vec2<T> operator-(const Vec2<U> &other) const 
    { 
        return { static_cast<T>(x - other.x), static_cast<T>(y - other.y) }; 
    }
    template <typename U>
    Vec2<T> operator*(const Vec2<U> &other) const 
    { 
        return { static_cast<T>(x * other.x), static_cast<T>(y * other.y) }; 
    }
    template <typename U>
    Vec2<T> operator/(const Vec2<U> &other) const 
    { 
        return { static_cast<T>(x / other.x), static_cast<T>(y / other.y) }; 
    }

    Vec2<T> operator*(const double &factor) const 
    { 
        return { static_cast<T>(x * factor), static_cast<T>(y * factor) }; 
    }
    Vec2<T> operator/(const double &factor) const 
    { 
        return { static_cast<T>(x / factor), static_cast<T>(y / factor) }; 
    }

    Vec2<T> operator-() const 
    { 
        return { static_cast<T>(-x), static_cast<T>(-y) }; 
    }

    template <typename U>
    Vec2<T>& operator+=(const Vec2<U> &other) 
    { 
        x += other.x; y += other.y; 
        return *this; 
    }
    template <typename U>
    Vec2<T>& operator-=(const Vec2<U> &other) 
    { 
        x -= other.x; y -= other.y; 
        return *this; 
    }
    template <typename U>
    Vec2<T>& operator*=(const Vec2<U> &other) 
    { 
        x *= other.x; y *= other.y; 
        return *this; 
    }
    template <typename U>
    Vec2<T>& operator/=(const Vec2<U> &other) 
    { 
        x /= other.x; y /= other.y; 
        return *this; 
    }

    Vec2<T>& operator*=(const double &factor) 
    { 
        x *= factor; y *= factor; 
        return *this; 
    }
    Vec2<T>& operator/=(const double &factor) 
    { 
        x /= factor; y /= factor; 
        return *this; 
    }

    template <typename U>
    Vec2<T> operator%(const Vec2<U> &other) const 
    {
        return { 
            static_cast<T>(static_cast<int64_t>(x) % static_cast<int64_t>(other.x)),
            static_cast<T>(static_cast<int64_t>(y) % static_cast<int64_t>(other.y)) 
        };
    }

    bool operator==(const Vec2<T> &other) const 
    { 
        return x == other.x && y == other.y; 
    }
    bool operator!=(const Vec2<T> &other) const 
    { 
        return x != other.x || y != other.y; 
    }
    void operator=(const Vec2<T> &other) 
    { 
        x = other.x; 
        y = other.y; 
    }

    template <typename U>
    operator Vec2<U>() const 
    { 
        return { static_cast<U>(x), static_cast<U>(y) }; 
    }
};

using Vec2i = Vec2<int>;
using Vec2d = Vec2<double>;
using Vec2f = Vec2<float>;

}

template <typename T>
struct std::hash<gfx::Vec2<T>>
{
    size_t operator()(const gfx::Vec2<T>& vec) const
    {
        return std::hash<T>()(vec.x) ^ (std::hash<T>()(vec.y) << 1);
    }
};
