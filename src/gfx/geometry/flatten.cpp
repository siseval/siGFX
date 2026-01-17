#include <stack>

#include "gfx/geometry/flatten.h"

namespace gfx
{

std::vector<Vec2d> Flatten::flatten_bezier(const Vec2d p0, const Vec2d p1, const Vec2d p2, const double tolerance)
{
    struct BezierSegment
    {
        double t0;
        double t1;
        Vec2d p0;
        Vec2d p1;
    };

    auto bezier_eval { [](const Vec2d &a, const Vec2d &b, const Vec2d &c, const double t) {
        const double u = 1.0 - t;
        return Vec2d {
            u * u * a.x + 2 * u * t * b.x + t * t * c.x,
            u * u * a.y + 2 * u * t * b.y + t * t * c.y
        };
    } };

    std::vector<Vec2d> points;
    std::stack<BezierSegment> segments;

    const Vec2d start { bezier_eval(p0, p1, p2, 0.0) };
    const Vec2d end { bezier_eval(p0, p1, p2, 1.0) };

    segments.push({ 0.0, 1.0, start, end });

    while (!segments.empty())
    {
        auto [t0, t1, p0, p1] { segments.top() };
        segments.pop();

        const double mid_t { (t0 + t1) / 2.0 };
        Vec2d mid_point { bezier_eval(p0, p1, p2, mid_t) };

        Vec2d line_mid { (p0 + p1) / 2.0 };
        const double distance { (mid_point - line_mid).length() };

        if (distance > tolerance)
        {
            segments.push({ mid_t, t1, mid_point, p1 });
            segments.push({ t0, mid_t, p0, mid_point });
        }
        else
        {
            if (points.empty() || points.back() != p0)
            {
                points.push_back(p0);
            }
            points.push_back(p1);
        }
    }

    points.push_back(end);
    return points;
}

std::vector<Vec2d> Flatten::flatten_contour(const std::vector<std::pair<Vec2d, bool>> &points_on_off_curve)
{
    std::vector<Vec2d> flattened_points;

    int start_index = 0;
    for (int i = 0; i < points_on_off_curve.size(); ++i)
    {
        if (points_on_off_curve[i].second)
        {
            start_index = i;
            break;
        }
    }

    Vec2d prev_on_point { points_on_off_curve[start_index].first.x, points_on_off_curve[start_index].first.y };
    flattened_points.push_back(prev_on_point);

    const int num_points { static_cast<int>(points_on_off_curve.size()) };

    for (int i = 1; i <= num_points; ++i)
    {
        const auto & [fst, snd] { points_on_off_curve[(start_index + i) % num_points] };

        if (snd)
        {
            Vec2d curr_on_point { fst.x, fst.y };
            flattened_points.push_back(curr_on_point);
            prev_on_point = curr_on_point;
        }
        else
        {
            const auto & [fst, snd] { points_on_off_curve[(start_index + i + 1) % num_points] };
            if (snd)
            {
                const Vec2d next_on_point { fst.x, fst.y };
                auto bezier_points { flatten_bezier(prev_on_point, Vec2d { fst.x, fst.y }, next_on_point) };
                flattened_points.insert(flattened_points.end(), bezier_points.begin() + 1, bezier_points.end());
                prev_on_point = next_on_point;
                ++i;
            }
            else
            {
                const Vec2d mid_point { (Vec2d { fst.x, fst.y } + Vec2d { fst.x, fst.y }) / 2.0 };
                auto bezier_points { flatten_bezier(prev_on_point, Vec2d { fst.x, fst.y }, mid_point) };
                flattened_points.insert(flattened_points.end(), bezier_points.begin() + 1, bezier_points.end());
                prev_on_point = mid_point;
            }
        }
    }
    return flattened_points;
}

}
