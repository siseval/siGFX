#pragma once

#include "gfx/core/primitive-2D.h"
#include "gfx/text/font-ttf.h"
#include "gfx/math/box2.h"
#include "gfx/math/matrix.h"
#include "gfx/math/vec2.h"
#include "gfx/text/utf-8.h"
#include "gfx/geometry/transform.h"

namespace gfx
{

class Text2D : public PrimitiveTemplate<Text2D>
{

public:

    enum class TextAlignment
    {
        LEFT,
        CENTER,
        RIGHT
    };


    Box2d get_geometry_size() const override;
    bool point_collides(const Vec2d point, const Matrix3x3d &transform) const override { return false; }

    void set_text(const std::string &new_text) 
    { 
        text = new_text; 
        set_edges_dirty();
        set_size_dirty();
    }

    void set_font(const std::shared_ptr<FontTTF> new_font) 
    { 
        font = new_font; 
        set_edges_dirty();
        set_size_dirty();
    }

    void set_font_size(const double new_font_size) 
    { 
        font_size = new_font_size; 
        set_edges_dirty();
        set_size_dirty();
    }

    void set_alignment(const TextAlignment new_alignment) 
    { 
        alignment = new_alignment; 
        set_size_dirty();
    }

    TextAlignment get_alignment() const { return alignment; }

    void set_smoothing_radius(const double radius) { smoothing_radius = radius; }
    double get_smoothing_radius() const { return smoothing_radius; }

    void set_line_height_multiplier(const double multiplier) 
    { 
        line_height_multiplier = multiplier; 
        set_size_dirty();
    }

    inline double get_line_height_multiplier() const { return line_height_multiplier; }

    template<typename EmitPixel>
    void rasterize_glyph_sdf(const std::vector<FontTTF::ContourEdge> &glyph, EmitPixel &&emit_pixel) const
    {
        if (glyph.empty())
        {
            return;
        }

        std::vector<EdgeData> edges = preprocess_edges(glyph);

        auto bounds = Box2i::unexpanded();
        for (const auto &e : glyph) 
        {
            bounds.expand(e.v0);
            bounds.expand(e.v1);
        }

        for (int y = bounds.min.y - 1; y <= bounds.max.y + 1; ++y) 
        {
            for (int x = bounds.min.x - 1; x <= bounds.max.x + 1; ++x) 
            {
                Vec2d p { x + 0.5, y + 0.5 };

                int winding = 0;
                for (const auto &e : edges) 
                {
                    bool intersects_y { e.p0.y > p.y != e.p1.y > p.y };
                    if (intersects_y)
                    {
                        double t = (p.y - e.p0.y) / (e.p1.y - e.p0.y);
                        double x_cross = e.p0.x + t * (e.p1.x - e.p0.x);
                        if (p.x < x_cross)
                        {
                            winding += (e.p1.y > e.p0.y) ? 1 : -1;
                        }
                    }
                }
                bool inside = (winding != 0);

                double signed_distance { signed_distance_to_glyph(edges, p, inside) };

                double coverage = coverage_from_sdf(signed_distance);

                if (coverage > 0.0) 
                {
                    emit_pixel({ 
                        { x, y }, 
                        color.set_alpha(coverage_to_alpha(coverage))
                    });
                }
            }
        }
    }
    
    template<typename EmitPixel>
    void rasterize_glyph(std::vector<FontTTF::ContourEdge> glyph, EmitPixel &&emit_pixel) const
    {
        if (glyph.empty()) 
        {
            return;
        }

        std::vector<EdgeData> edges = preprocess_edges(glyph);

        auto bounds { Box2i::unexpanded() };

        for (const auto &edge : glyph)
        {
            bounds.expand(edge.v0);
            bounds.expand(edge.v1);
        }

        std::vector<std::vector<ETEntry>> edge_table {
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

            AET.erase(
                std::remove_if(AET.begin(), AET.end(), [y](const ETEntry &e){ 
                    return y >= e.y_max; 
                }),
                AET.end()
            );

            std::sort(AET.begin(), AET.end(), [](const ETEntry &a, const ETEntry &b){ 
                return a.x < b.x; 
            });

            for (int i = 0; i + 1 < AET.size(); i += 2)
            {
                int x0 = static_cast<int>(std::ceil(AET[i].x));
                int x1 = static_cast<int>(std::floor(AET[i + 1].x));

                for (int x = x0; x <= x1; ++x)
                {
                    Vec2d p { x + 0.5, y + 0.5 };
                    emit_pixel({ { x, y }, color });
                }
            }

            for (auto &edge : AET)
            {
                edge.x += edge.dx;
            }
        }
    }

    template<typename EmitPixel>
    void rasterize(const Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        double scale = font_size / font->get_units_per_em();
        double ascent = font->get_ascent() * scale;
        double line_gap = font->get_line_gap() * scale;

        double line_height = (line_gap > 0.0) ? 
            font_size + line_gap : 
            font_size * line_height_multiplier;

        Vec2d pen { Vec2d::zero() };

        Vec2d min { Vec2d(std::numeric_limits<double>::max()) };
        Vec2d max { Vec2d(std::numeric_limits<double>::lowest()) };

        std::vector<double> line_widths { 0.0 };
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

            auto edges = font->get_glyph_edges(codepoint);
            for (auto &edge : edges)
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

            double offset_x { [&] { switch (alignment) 
                {
                    case TextAlignment::LEFT: return 0.0;
                    case TextAlignment::RIGHT: return max.x - line_widths[line_index];
                    case TextAlignment::CENTER: return (max.x - line_widths[line_index]) / 2.0;
                }
                std::unreachable();
            }()};

            auto edges = font->get_glyph_edges(codepoint);
            for (auto &edge : edges)
            {
                edge.v0 = edge.v0 * scale;
                edge.v1 = edge.v1 * scale;

                edge.v0.x += pen.x - min.x + offset_x;
                edge.v1.x += pen.x - min.x + offset_x;

                edge.v0.y = -edge.v0.y + ascent + pen.y - min.y;
                edge.v1.y = -edge.v1.y + ascent + pen.y - min.y;

                edge.v0 = Transform::transform_point(edge.v0, transform);
                edge.v1 = Transform::transform_point(edge.v1, transform);
            }

            if (smoothing_radius > 0.0)
            {
                rasterize_glyph_sdf(edges, emit_pixel);
            }
            else
            {
                rasterize_glyph(edges, emit_pixel);
            }

            pen.x += font->get_glyph_advance(codepoint) * scale;
            i += bytes;
        }
    }

private:

    struct EdgeData
    {
        Vec2d p0;
        Vec2d p1;
        Vec2d dir;
        Vec2d normal;
        double inv_length_sq;
    };

    struct ETEntry 
    {
        double x;
        double dx;
        int y_max;
    };

    std::vector<EdgeData> preprocess_edges(const std::vector<FontTTF::ContourEdge> &edges) const;
    std::vector<std::vector<ETEntry>> build_edge_table(const std::vector<FontTTF::ContourEdge> &edges, const Box2i &bounds) const;
    bool point_inside_glyph(const std::vector<ETEntry> &edges, const Vec2d point) const;
    double dist_to_segment(const EdgeData &edge_data, const Vec2d point) const;
    double signed_distance_to_glyph(const std::vector<EdgeData> &edges, const Vec2d &point, const bool inside) const;
    double coverage_from_sdf(const double signed_distance) const;
    double coverage_to_alpha(const double coverage) const;

    void set_edges_dirty() { edges_dirty = true; }
    void set_size_dirty() { size_dirty = true; }

    TextAlignment alignment = TextAlignment::LEFT;

    Vec2d text_box { -1.0, -1.0 };

    std::string text;
    std::shared_ptr<FontTTF> font;

    double font_size;
    double line_height_multiplier = 1.2;

    double smoothing_radius = 0.0;

    mutable bool edges_dirty = true;
    mutable bool size_dirty = true;

    mutable Box2d cached_geometry_size;
    mutable std::unordered_map<uint32_t, std::vector<FontTTF::ContourEdge>> cached_glyph_edges;
 };

}
