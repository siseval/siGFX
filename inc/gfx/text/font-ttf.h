#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "gfx/text/font.h"
#include "gfx/math/box2.h"

namespace gfx
{

class FontTTF
{

public:

    struct Point
    {
        double x;
        double y;
        bool on_curve;
    };

    struct GlyphTTF
    {
        Box2d bbox;
        std::vector<std::vector<Point>> contours;
    };

    struct ContourEdge
    {
        Vec2d v0;
        Vec2d v1;
    };

    struct GlyphMetrics
    {
        int advance_width;
        int left_side_bearing;
    };

    FontTTF(int units_per_em, double ascent, double descent, double line_gap, int num_glyphs);

    std::shared_ptr<GlyphTTF> get_glyph(const uint32_t codepoint) const;
    std::vector<ContourEdge> get_glyph_edges(const uint32_t codepoint) const;

    void set_kerning(const char left, const char right, int offset);
    int get_kerning(const char left, const char right) const;

    void set_metrics(const uint32_t codepoint, const GlyphMetrics &metrics);
    int get_glyph_advance(const uint32_t codepoint) const;
    int get_glyph_left_side_bearing(const uint32_t codepoint) const;

    double get_ascent() const;
    double get_descent() const;
    double get_line_gap() const;
    double get_line_height() const;
    double get_units_per_em() const;

    void set_name(const std::string &n);
    std::string get_name() const;

    void set_glyphs(const std::unordered_map<uint32_t, std::shared_ptr<GlyphTTF>> &g);
    std::unordered_map<uint32_t, std::shared_ptr<GlyphTTF>> get_glyphs() const;

private:

    std::vector<ContourEdge> flatten_glyph(const std::shared_ptr<GlyphTTF> glyph) const;
    bool decode_utf8(const std::string &s, size_t pos, uint32_t &out_codepoint, size_t &bytes) const;
    std::string name;

    int units_per_em;
    double ascent;
    double descent;
    double line_gap;
    int num_glyphs;

    struct PairHash {
        std::size_t operator()(const std::pair<int,int>& p) const 
        {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    std::unordered_map<uint32_t, std::shared_ptr<GlyphTTF>> glyphs;
    std::unordered_map<uint32_t, GlyphMetrics> glyph_metrics;
    std::unordered_map<std::pair<uint32_t, uint32_t>, int, PairHash> kerning_table;

    mutable std::unordered_map<uint32_t, std::vector<ContourEdge>> vertex_cache;

};

}
