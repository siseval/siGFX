#include "gfx/text/font-ttf.h"
#include "gfx/geometry/flatten.h"

namespace gfx
{

FontTTF::FontTTF(int units_per_em, double ascent, double descent, double line_gap, int num_glyphs)
    : units_per_em(units_per_em), ascent(ascent), descent(descent), line_gap(line_gap), num_glyphs(num_glyphs)
{
}

std::shared_ptr<FontTTF::GlyphTTF> FontTTF::get_glyph(const uint32_t codepoint) const
{
    auto it = glyphs.find(codepoint);
    if (it != glyphs.end())
    {
        return it->second;
    }
    return nullptr;
}

std::vector<FontTTF::ContourEdge> FontTTF::get_glyph_edges(const uint32_t codepoint) const
{
    auto it = vertex_cache.find(codepoint);
    if (it != vertex_cache.end())
    {
        return it->second;
    }

    std::vector<FontTTF::ContourEdge> vertices = flatten_glyph(get_glyph(codepoint));
    vertex_cache[codepoint] = vertices;
    return vertices;
}

std::vector<FontTTF::ContourEdge> FontTTF::flatten_glyph(const std::shared_ptr<FontTTF::GlyphTTF> glyph) const
{
    std::vector<FontTTF::ContourEdge> edges;
    if (!glyph)
    {
        return edges;
    }

    for (const auto &contour : glyph->contours)
    {
        std::vector<std::pair<Vec2d, bool>> points_on_off_curve;
        for (const auto &point : contour)
        {
            points_on_off_curve.push_back({ Vec2d { point.x, point.y }, point.on_curve });
        }

        std::vector<Vec2d> flattened_contour = Flatten::flatten_contour(points_on_off_curve);
        int num_points = flattened_contour.size();
        for (int i = 0; i < num_points; ++i)
        {
            FontTTF::ContourEdge edge {
                flattened_contour[i],
                flattened_contour[(i + 1) % num_points]
            };
            edges.push_back(edge);
        }
    }
    return edges;
}

void FontTTF::set_kerning(const char left, const char right, int offset)
{
    kerning_table[{static_cast<uint32_t>(static_cast<uint8_t>(left)), static_cast<uint32_t>(static_cast<uint8_t>(right))}] = offset;
}

int FontTTF::get_kerning(const char left, const char right) const
{
    return kerning_table.count({static_cast<uint32_t>(static_cast<uint8_t>(left)), static_cast<uint32_t>(static_cast<uint8_t>(right))}) ?
        kerning_table.at({static_cast<uint32_t>(static_cast<uint8_t>(left)), static_cast<uint32_t>(static_cast<uint8_t>(right))}) : 0;
}

void FontTTF::set_metrics(const uint32_t codepoint, const GlyphMetrics &metrics)
{
    glyph_metrics[codepoint] = metrics;
}

int FontTTF::get_glyph_advance(const uint32_t codepoint) const
{
    return glyph_metrics.count(codepoint) ? glyph_metrics.at(codepoint).advance_width : 0;
}

int FontTTF::get_glyph_left_side_bearing(const uint32_t codepoint) const
{
    return glyph_metrics.count(codepoint) ? glyph_metrics.at(codepoint).left_side_bearing : 0;
}

double FontTTF::get_ascent() const
{
    return ascent;
}

double FontTTF::get_descent() const
{
    return descent;
}

double FontTTF::get_line_gap() const
{
    return line_gap;
}

double FontTTF::get_line_height() const
{
    return ascent - descent + line_gap;
}

double FontTTF::get_units_per_em() const
{
    return units_per_em;
}

void FontTTF::set_name(const std::string &n)
{
    name = n;
}

std::string FontTTF::get_name() const
{
    return name;
}

void FontTTF::set_glyphs(const std::unordered_map<uint32_t, std::shared_ptr<GlyphTTF>> &g)
{
    glyphs = g;
}

std::unordered_map<uint32_t, std::shared_ptr<FontTTF::GlyphTTF>> FontTTF::get_glyphs() const
{
    return glyphs;
}

}
