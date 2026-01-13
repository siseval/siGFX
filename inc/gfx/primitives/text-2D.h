#pragma once

#include "gfx/core/primitive-2D.h"
#include "gfx/text/font-ttf.h"
#include "gfx/math/box2.h"
#include "gfx/math/matrix.h"
#include "gfx/math/vec2.h"

namespace gfx
{

class Text2D : public Primitive2D
{

public:

    enum class TextAlignment
    {
        LEFT,
        CENTER,
        RIGHT
    };

    std::vector<Vec2i> rasterize(const Matrix3x3d &transform) const override;

    Box2d get_geometry_size() const override;

    void set_text(const std::string &new_text);
    void set_font(const std::shared_ptr<FontTTF> new_font);
    void set_font_size(const double new_font_size);
    void set_alignment(const TextAlignment new_alignment);

    TextAlignment get_alignment() const;

    void set_smoothing_radius(const double radius);
    double get_smoothing_radius() const;

    void set_line_height_multiplier(const double multiplier);
    inline double get_line_height_multiplier() const;

    void rasterize_glyph_sdf(const std::vector<FontTTF::ContourEdge> &glyph, std::vector<Vec2i> &pixels) const;
    void rasterize_glyph(std::vector<FontTTF::ContourEdge> glyph, std::vector<Vec2i> &pixels) const;

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

    void set_edges_dirty();
    void set_size_dirty();

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
