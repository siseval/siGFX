#include "gfx/primitives/text-2D.h"

#include "gfx/geometry/transform-2D.h"
#include "gfx/text/utf-8.h"


namespace gfx
{

Primitive2D::RasterizeOutput Text2D::rasterize(const Matrix3x3d &transform) const
{
    RasterizeOutput output;

    const double scale = font_size / font->get_units_per_em();
    const double ascent = font->get_ascent() * scale;
    const double line_gap = font->get_line_gap() * scale;

    const double line_height = line_gap > 0.0 ?
                                   font_size + line_gap :
                                   font_size * line_height_multiplier;

    Vec2d pen { Vec2d::zero() };

    auto min { Vec2d(std::numeric_limits<double>::max()) };
    auto max { Vec2d(std::numeric_limits<double>::lowest()) };

    std::vector line_widths { 0.0 };
    int line_index = 0;

    size_t i = 0;
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
            i += bytes;
            line_index++;
            line_widths.push_back(0.0);
            continue;
        }

        if (i > 0)
        {
            size_t prev_bytes;
            uint32_t prev_cp;
            decode_utf8(text, i - 1, prev_cp, prev_bytes);
            pen.x += font->get_kerning(prev_cp, codepoint) * scale;
            line_widths[line_index] = pen.x;
        }

        for (auto edges = font->get_glyph_edges(codepoint); auto &edge : edges)
        {
            Vec2d v0 = edge.v0 * scale;
            Vec2d v1 = edge.v1 * scale;

            v0.x += pen.x; v1.x += pen.x;
            v0.y = -v0.y + ascent + pen.y;
            v1.y = -v1.y + ascent + pen.y;

            min.x = std::min({min.x, v0.x, v1.x});
            min.y = std::min({min.y, v0.y, v1.y});

            max.x = std::max({max.x, v0.x, v1.x});
            max.y = std::max({max.y, v0.y, v1.y});
        }

        pen.x += font->get_glyph_advance(codepoint) * scale;
        line_widths[line_index] = pen.x;
        i += bytes;
    }

    pen = Vec2d { 0.0, 0.0 };
    line_index = 0;
    i = 0;
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
            i += bytes;
            line_index++;
            continue;
        }

        if (i > 0)
        {
            size_t prev_bytes;
            uint32_t prev_cp;
            decode_utf8(text, i - 1, prev_cp, prev_bytes);
            pen.x += font->get_kerning(prev_cp, codepoint) * scale;
        }

        const double offset_x { [&] { switch (alignment)
            {
                case TextAlignment::LEFT: return 0.0;
                case TextAlignment::RIGHT: return max.x - line_widths[line_index];
                case TextAlignment::CENTER: return (max.x - line_widths[line_index]) / 2.0;
            }
            std::unreachable();
        }()};

        auto edges = font->get_glyph_edges(codepoint);
        for (auto & [v0, v1] : edges)
        {
            v0 = v0 * scale;
            v1 = v1 * scale;

            v0.x += pen.x - min.x + offset_x;
            v1.x += pen.x - min.x + offset_x;

            v0.y = -v0.y + ascent + pen.y - min.y;
            v1.y = -v1.y + ascent + pen.y - min.y;

            v0 = Transform2D::transform_point(v0, transform);
            v1 = Transform2D::transform_point(v1, transform);
        }

        if (smoothing_radius > 0.0)
        {
            rasterize_glyph_sdf(edges, output);
        }
        else
        {
            rasterize_glyph(edges, output.pixels);
        }

        pen.x += font->get_glyph_advance(codepoint) * scale;
        i += bytes;
    }

    if (smoothing_radius > 0.0)
    {
        output.use_alphas = true;
    }

    return output;
}

Box2d Text2D::get_geometry_size() const
{
    const double scale = font_size / font->get_units_per_em();
    const double ascent = font->get_ascent() * scale;
    const double line_gap = font->get_line_gap() * scale;

    const double line_height = line_gap > 0.0 ? font_size + line_gap : font_size * line_height_multiplier;

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

        for (auto edges = font->get_glyph_edges(codepoint); auto &edge : edges)
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


std::vector<Text2D::EdgeData> Text2D::preprocess_edges(const std::vector<FontTTF::ContourEdge> &edges)
{
    std::vector<EdgeData> data;
    data.reserve(edges.size());

    for (const auto & [v0, v1] : edges)
    {
        Vec2d v { v1 - v0 };
        const double len2 { Vec2d::dot(v, v) };
        double inv_len { 1.0 / std::sqrt(len2) };

        const Vec2d dir { v * inv_len };
        const Vec2d normal { -dir.y, dir.x };

        data.emplace_back(EdgeData {
            v0,
            v1,
            dir,
            normal,
            1.0 / len2
        });
    }

    return data;
}

std::vector<std::vector<Text2D::ETEntry>> Text2D::build_edge_table(const std::vector<FontTTF::ContourEdge> &edges, const Box2i &bounds)
{
    std::vector<std::vector<ETEntry>> edge_table(bounds.max.y - bounds.min.y + 1);

    for (const auto& [v0, v1] : edges)
    {
        double y0 = v0.y;
        double y1 = v1.y;
        double x0 = v0.x;
        double x1 = v1.x;

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

        const double dy = y1 - y0;
        const double dx = (x1 - x0) / dy;

        const double intersection = x0 + (y_start - y0) * dx;

        ETEntry entry {
            .x = intersection,
            .dx = dx,
            .y_max = y_end
        };

        edge_table[y_start - bounds.min.y].push_back(entry);
    }

    for (auto &entries : edge_table)
    {
        std::ranges::sort(entries, [](const ETEntry &a, const ETEntry &b) {
            return a.x < b.x;
        });
    }

    return edge_table;
}

bool Text2D::point_inside_glyph(const std::vector<ETEntry> &edges, const Vec2d point) 
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

double Text2D::dist_to_segment(const EdgeData &edge_data, const Vec2d point)
{
    const Vec2d w { point - edge_data.p0 };
    const double t { std::clamp(
        Vec2d::dot(w, edge_data.dir), 0.0,
        (edge_data.p1 - edge_data.p0).length())
    };

    const Vec2d dist { point - (edge_data.p0 + edge_data.dir * t) };

    return std::sqrt(Vec2d::dot(dist, dist));
}

double Text2D::signed_distance_to_glyph(const std::vector<EdgeData> &edges, const Vec2d &point, const bool inside)
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
    const double a = smoothing_radius - signed_distance / (2.0 * smoothing_radius);
    return std::clamp(a, 0.0, 1.0);
}

double Text2D::coverage_to_alpha(const double coverage)
{
    // return coverage;
    // return std::pow(coverage, 1.5);
    return coverage * coverage * (3.0 - 2.0 * coverage);
}

void Text2D::rasterize_glyph_sdf(const std::vector<FontTTF::ContourEdge> &glyph, RasterizeOutput &output) const
{
    if (glyph.empty())
    {
        return;
    }

    const std::vector<EdgeData> edges = preprocess_edges(glyph);

    auto bounds = Box2i::unexpanded();
    for (const auto & [v0, v1] : glyph)
    {
        bounds.expand(v0);
        bounds.expand(v1);
    }

    for (int y = bounds.min.y - 1; y <= bounds.max.y + 1; ++y)
    {
        for (int x = bounds.min.x - 1; x <= bounds.max.x + 1; ++x)
        {
            Vec2d p { x + 0.5, y + 0.5 };

            int winding = 0;
            for (const auto & [p0, p1, dir, normal, inv_length_sq] : edges)
            {
                const bool intersects_y { p0.y > p.y != p1.y > p.y };
                if (intersects_y)
                {
                    const double t = (p.y - p0.y) / (p1.y - p0.y);
                    const double x_cross = p0.x + t * (p1.x - p0.x);
                    if (p.x < x_cross)
                    {
                        winding += p1.y > p0.y ? 1 : -1;
                    }
                }
            }
            const bool inside = winding != 0;

            const double signed_distance { signed_distance_to_glyph(edges, p, inside) };

            const double coverage = coverage_from_sdf(signed_distance);

            if (coverage > 0.0)
            {
                output.pixels.push_back(Vec2i { x, y });
                output.alphas.push_back(coverage_to_alpha(coverage));
            }
        }
    }
}

void Text2D::rasterize_glyph(std::vector<FontTTF::ContourEdge> glyph, std::vector<Vec2i> &pixels)
{
    if (glyph.empty())
    {
        return;
    }

    std::vector<EdgeData> edges = preprocess_edges(glyph);

    auto bounds { Box2i::unexpanded() };

    for (const auto & [v0, v1] : glyph)
    {
        bounds.expand(v0);
        bounds.expand(v1);
    }

    const std::vector edge_table {
        build_edge_table(glyph, bounds)
    };

    std::vector<ETEntry> AET;

    for (int y = bounds.min.y; y <= bounds.max.y; ++y)
    {
        if (y - bounds.min.y < edge_table.size())
        {
            for (const auto &edge : edge_table[y - bounds.min.y])
            {
                AET.push_back(edge);
            }
        }

        std::erase_if(AET, [y](const ETEntry &e){
            return y >= e.y_max;
        });

        std::ranges::sort(AET, [](const ETEntry &a, const ETEntry &b){
            return a.x < b.x;
        });

        for (int i = 0; i + 1 < AET.size(); i += 2)
        {
            const int x0 = static_cast<int>(std::ceil(AET[i].x));
            const int x1 = static_cast<int>(std::floor(AET[i + 1].x));

            for (int x = x0; x <= x1; ++x)
            {
                pixels.push_back(Vec2i { x, y });
            }
        }

        for (auto &edge : AET)
        {
            edge.x += edge.dx;
        }
    }
}

void Text2D::set_text(const std::string &new_text)
{
    text = new_text;
    set_edges_dirty();
    set_size_dirty();
}

void Text2D::set_font(const std::shared_ptr<FontTTF> new_font)
{
    font = new_font;
    set_edges_dirty();
    set_size_dirty();
}

void Text2D::set_font_size(const double new_font_size)
{
    font_size = new_font_size;
    set_edges_dirty();
    set_size_dirty();
}

void Text2D::set_alignment(const TextAlignment new_alignment)
{
    alignment = new_alignment;
    set_size_dirty();
}

Text2D::TextAlignment Text2D::get_alignment() const
{
    return alignment;
}

void Text2D::set_smoothing_radius(const double radius)
{
    smoothing_radius = radius;
}

double Text2D::get_smoothing_radius() const
{
    return smoothing_radius;
}

void Text2D::set_line_height_multiplier(const double multiplier)
{
    line_height_multiplier = multiplier;
    set_size_dirty();
}

double Text2D::get_line_height_multiplier() const
{
    return line_height_multiplier;
}

void Text2D::set_edges_dirty() const
{
    edges_dirty = true;
}

void Text2D::set_size_dirty() const
{
    size_dirty = true;
}


}
