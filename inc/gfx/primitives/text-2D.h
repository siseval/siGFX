#ifndef TEXT_2D_H
#define TEXT_2D_H

#include <gfx/core/primitive-2D.h>
#include <gfx/text/font-ttf.h>
#include <gfx/math/box2.h>
#include <gfx/math/matrix.h>
#include <gfx/math/vec2.h>
#include <gfx/text/utf-8.h>
#include <gfx/utils/transform.h>

namespace gfx::primitives
{

class Text2D : public gfx::core::PrimitiveTemplate<Text2D>
{

public:

    enum class TextAlignment
    {
        LEFT,
        CENTER,
        RIGHT
    };

    gfx::math::Box2d get_geometry_size() const override;
    bool point_collides(const gfx::math::Vec2d point, const gfx::math::Matrix3x3d &transform) const override { return false; }

    void set_text(const std::string &new_text) 
    { 
        text = new_text; 
        set_edges_dirty();
        set_size_dirty();
    }

    void set_font(const std::shared_ptr<gfx::text::FontTTF> new_font) 
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

    inline void set_line_height_multiplier(const double multiplier) 
    { 
        line_height_multiplier = multiplier; 
        set_size_dirty();
    }

    inline double get_line_height_multiplier() const { return line_height_multiplier; }
    
    template<typename EmitPixel>
    void rasterize_glyph(std::vector<gfx::text::ContourEdge> glyph, EmitPixel &&emit_pixel) const
    {
        if (glyph.empty()) 
        {
            return;
        }

        gfx::math::Box2i bounds { glyph[0].v0.round(), glyph[0].v0.round() };

        for (auto &edge : glyph) 
        {
            bounds.expand(edge.v0);
            bounds.expand(edge.v1);
        }

        const int height = bounds.max.y - bounds.min.y + 1;

        std::vector<std::vector<size_t>> edge_table(height);

        for (size_t i = 0; i < glyph.size(); ++i) 
        {
            const auto &edge = glyph[i];

            if (edge.v0.y == edge.v1.y) 
            {
                continue;
            }

            int y0 = static_cast<int>(std::floor(std::min(edge.v0.y, edge.v1.y)));
            int y1 = static_cast<int>(std::ceil(std::max(edge.v0.y, edge.v1.y)));

            for (int y = y0; y < y1; ++y)
            {
                if (y >= bounds.min.y && y <= bounds.max.y)
                {
                    edge_table[y - bounds.min.y].push_back(i);
                }
            }
        }

        for (int y = bounds.min.y; y <= bounds.max.y; ++y)
        {
            const auto &row_edges = edge_table[y - bounds.min.y];

            if (row_edges.empty()) 
            {
                continue;
            }

            std::vector<double> intersections;
            intersections.reserve(row_edges.size());

            for (size_t index : row_edges)
            {
                const auto &edge = glyph[index];

                double y0 = edge.v0.y;
                double y1 = edge.v1.y;

                if ((y < y0 && y < y1) || (y >= y0 && y >= y1)) 
                {
                    continue;
                }

                double t = (y - y0) / (y1 - y0);
                intersections.push_back(edge.v0.x + t * (edge.v1.x - edge.v0.x));
            }

            std::sort(intersections.begin(), intersections.end());

            for (size_t i = 0; i + 1 < intersections.size(); i += 2)
            {
                int x0 = static_cast<int>(std::ceil(intersections[i]));
                int x1 = static_cast<int>(std::floor(intersections[i + 1]));
                for (int x = x0; x <= x1; ++x)
                {
                    emit_pixel( { { x, y }, color });
                }
            }
        }
    }

    template<typename EmitPixel>
    void rasterize(const gfx::math::Matrix3x3d &transform, EmitPixel &&emit_pixel) const
    {
        double scale = font_size / font->get_units_per_em();
        double ascent = font->get_ascent() * scale;
        double line_gap = font->get_line_gap() * scale;

        double line_height = (line_gap > 0.0) ? 
            font_size + line_gap : 
            font_size * line_height_multiplier;

        gfx::math::Vec2d pen { gfx::math::Vec2d::zero() };

        gfx::math::Vec2d min { gfx::math::Vec2d(std::numeric_limits<double>::max()) };
        gfx::math::Vec2d max { gfx::math::Vec2d(std::numeric_limits<double>::lowest()) };

        std::vector<double> line_widths { 0.0 };
        int line_index = 0;

        size_t i = 0;
        while (i < text.size())
        {
            uint32_t codepoint;
            size_t bytes;

            if (!gfx::text::decode_utf8(text, i, codepoint, bytes))
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
                gfx::text::decode_utf8(text, i - 1, prev_cp, prev_bytes);
                pen.x += font->get_kerning(prev_cp, codepoint) * scale;
                line_widths[line_index] = pen.x;
            }

            auto edges = font->get_glyph_edges(codepoint);
            for (auto &edge : edges)
            {
                gfx::math::Vec2d v0 = edge.v0 * scale;
                gfx::math::Vec2d v1 = edge.v1 * scale;

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

        pen = gfx::math::Vec2d { 0.0, 0.0 };
        line_index = 0;
        i = 0;
        while (i < text.size())
        {
            uint32_t codepoint;
            size_t bytes;

            if (!gfx::text::decode_utf8(text, i, codepoint, bytes))
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
                gfx::text::decode_utf8(text, i - 1, prev_cp, prev_bytes);
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

                edge.v0 = utils::transform_point(edge.v0, transform);
                edge.v1 = utils::transform_point(edge.v1, transform);
            }

            rasterize_glyph(edges, emit_pixel);
            pen.x += font->get_glyph_advance(codepoint) * scale;
            i += bytes;
        }
    }

private:

    void set_edges_dirty() { edges_dirty = true; }
    void set_size_dirty() { size_dirty = true; }

    TextAlignment alignment = TextAlignment::LEFT;

    gfx::math::Vec2d text_box { -1.0, -1.0 };

    std::string text;
    std::shared_ptr<gfx::text::FontTTF> font;

    double font_size;
    double line_height_multiplier = 1.2;

    mutable bool edges_dirty = true;
    mutable bool size_dirty = true;

    mutable gfx::math::Box2d cached_geometry_size;
    mutable std::unordered_map<uint32_t, std::vector<gfx::text::ContourEdge>> cached_glyph_edges;
 };

}

#endif // TEXT_2D_H
