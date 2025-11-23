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


}
