#include <gfx/primitives/text-2D.h>
#include <gfx/utils/transform.h>
#include <gfx/text/utf-8.h>

namespace gfx::primitives
{

using namespace gfx::math;
using namespace gfx::core::types;
using namespace gfx::text;


Box2d Text2D::get_geometry_size() const
{
    double scale = font_size / font->get_units_per_em();
    double ascent = font->get_ascent() * scale;
    double line_gap = font->get_line_gap() * scale;

    double line_height = (line_gap > 0.0) ? font_size + line_gap : font_size * line_height_multiplier;

    Box2d bounds {
        Vec2d(std::numeric_limits<double>::max()),
        Vec2d(std::numeric_limits<double>::lowest())
    };

    Vec2d pen {0.0, 0.0};

    size_t i = 0;
    uint32_t prev_codepoint = 0;

    while (i < text.size())
    {
        uint32_t codepoint;
        size_t bytes;

        if (!decode_utf8(text, i, codepoint, bytes))
        {
            ++i;
            continue;
        }

        if (codepoint == '\n')
        {
            pen.x = 0.0;
            pen.y += line_height;
            prev_codepoint = 0;
            i += bytes;
            continue;
        }

        if (prev_codepoint != 0)
        {
            pen.x += font->get_kerning(prev_codepoint, codepoint) * scale;
        }

        auto edges = font->get_glyph_edges(codepoint);
        for (auto &edge : edges)
        {
            Vec2d v0 = edge.v0 * scale;
            Vec2d v1 = edge.v1 * scale;

            v0.x += pen.x;
            v1.x += pen.x;
            v0.y = -v0.y + ascent + pen.y;
            v1.y = -v1.y + ascent + pen.y;

            bounds.expand(v0);
            bounds.expand(v1);
        }

        pen.x += font->get_glyph_advance(codepoint) * scale;
        prev_codepoint = codepoint;
        i += bytes;
    }

    return Box2d { Vec2d::zero(), bounds.size() };
}


std::vector<Text2D::EdgeData> Text2D::preprocess_edges(const std::vector<ContourEdge> &edges) const
{
    std::vector<EdgeData> data;
    data.reserve(edges.size());

    for (const auto &edge : edges)
    {
        Vec2d v { edge.v1 - edge.v0 };
        double len2 { Vec2d::dot(v, v) };
        double inv_len { 1.0 / std::sqrt(len2) };

        Vec2d dir { v * inv_len };
        Vec2d normal { -dir.y, dir.x };

        data.emplace_back(EdgeData {
            edge.v0,
            edge.v1,
            dir,
            normal,
            1.0 / len2
        });
    }

    return data;
}

std::vector<std::vector<Text2D::ETEntry>> Text2D::build_edge_table(const std::vector<ContourEdge> &glyph, const Box2i &bounds) const
{
    std::vector<std::vector<ETEntry>> edge_table(bounds.max.y - bounds.min.y + 1);

    for (const auto& edge : glyph)
    {
        double y0 = edge.v0.y;
        double y1 = edge.v1.y;
        double x0 = edge.v0.x;
        double x1 = edge.v1.x;

        if (y0 == y1)
        {
            continue;
        }

        if (y0 > y1) 
        {
            std::swap(y0, y1);
            std::swap(x0, x1);
        }

        int y_start = static_cast<int>(std::ceil(y0));
        int y_end   = static_cast<int>(std::ceil(y1));

        if (y_end <= bounds.min.y || y_start > bounds.max.y)
        {
            continue;
        }

        y_start = std::max(y_start, bounds.min.y);
        y_end   = std::min(y_end, bounds.max.y + 1);

        double dy = y1 - y0;
        double dx = (x1 - x0) / dy;

        double intersection = x0 + (y_start - y0) * dx;

        ETEntry entry {
            .x = intersection,
            .dx = dx,
            .y_max = y_end
        };
        
        edge_table[y_start - bounds.min.y].push_back(entry);
    }

    for (auto &entries : edge_table)
    {
        std::sort(entries.begin(), entries.end(), [](const ETEntry &a, const ETEntry &b) {
            return a.x < b.x;
        });
    }

    return edge_table;
}

bool Text2D::point_inside_glyph(const std::vector<Text2D::ETEntry> &edges, const gfx::math::Vec2d point) const
{
    if (edges.empty())
    {
        return false;
    }

    bool inside = false;
    for (size_t i = 0; i < edges.size(); i += 2)
    {
        if (point.x >= edges[i].x && point.x < edges[i + 1].x)
        {
            inside = true;
            break;
        }
    }

    return inside;
}

double Text2D::dist_to_segment(const EdgeData &edge_data, const Vec2d point) const
{
    Vec2d w { point - edge_data.p0 };
    double t { std::clamp(
        Vec2d::dot(w, edge_data.dir), 0.0, 
        (edge_data.p1 - edge_data.p0).length()) 
    };

    Vec2d dist { point - (edge_data.p0 + edge_data.dir * t) };

    return std::sqrt(Vec2d::dot(dist, dist));
}

double Text2D::signed_distance_to_glyph(const std::vector<EdgeData> &edges, const Vec2d &point, const bool inside) const
{
    double dist { std::numeric_limits<double>::max() };

    for (const auto &edge : edges)
    {
        dist = std::min(dist, dist_to_segment(edge, point));
    }

    return inside ? -dist : dist;
}


double Text2D::coverage_from_sdf(const double signed_distance) const
{
    double a = smoothing_radius - signed_distance / (2.0 * smoothing_radius);
    return std::clamp(a, 0.0, 1.0);
}

double Text2D::coverage_to_alpha(const double coverage) const
{
    // return coverage;
    // return std::pow(coverage, 1.5);
    return coverage * coverage * (3.0 - 2.0 * coverage);
}


}
