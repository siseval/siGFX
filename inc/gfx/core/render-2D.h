#pragma once

#include "gfx/core/scene-graph-2D.h"
#include "gfx/core/render-surface.h"
#include "gfx/core/types/color4.h"
// #include "gfx/core/types/bitmap.h"
#include "gfx/primitives/circle-2D.h"
#include "gfx/primitives/ellipse-2D.h"
#include "gfx/primitives/polyline-2D.h"
#include "gfx/primitives/polygon-2D.h"
#include "gfx/primitives/text-2D.h"
#include "gfx/primitives/bitmap-2D.h"
#include "gfx/text/font-manager-ttf.h"
#include "gfx/debug/debug-viewer.h"

namespace gfx
{

class Render2D
{

public:

    Render2D(std::shared_ptr<RenderSurface> surface, Vec2d viewport_scaling = Vec2d { 1, 1 });

    void draw_frame() const;
    void clear_frame() const;
    void present_frame() const;

    void add_item(const std::shared_ptr<Primitive2D> item);
    void add_item(const std::shared_ptr<Primitive2D> item, const std::shared_ptr<Primitive2D> parent);
    void remove_item(const std::shared_ptr<Primitive2D> item);
    void clear_items();

    int num_items() const;
    bool contains_item(const std::shared_ptr<Primitive2D> item) const;

    void set_resolution(const Vec2i new_resolution);
    void set_resolution(const int width, const int height);
    void set_viewport_scaling(const Vec2d scaling);
    void set_viewport_scaling(const double x, const double y);
    void set_clear_color(const Color4 color);
    Color4 get_clear_color() const;
    void set_blend_mode(const RenderSurface::BlendMode mode);
    Matrix3x3d get_global_transform() const;

    void set_font_directory(const std::filesystem::path &path);
    void load_font_directory(const std::filesystem::path &path = "");

    void set_enable_debug_viewer(const bool enable);
    void set_debug_viewer_show_aabb(const bool show);
    void set_debug_viewer_show_obb(const bool show);
    void set_debug_viewer_show_anchor(const bool show);
    void set_debug_font(const std::shared_ptr<FontTTF> font);

    void set_render_surface(const std::shared_ptr<RenderSurface> new_surface);

    Vec2i get_resolution() const;
    Vec2d center() const;
    Vec2d get_aspect_ratio() const;
    Vec2d get_viewport_scaling() const;
    RenderSurface::BlendMode get_blend_mode() const;

    std::filesystem::path get_font_directory() const;
    bool is_font_loaded(const std::string &name) const;
    bool fonts_empty() const;
    std::shared_ptr<FontTTF> get_font(const std::string &name) const;

    bool get_enable_debug_viewer() const;
    bool get_debug_viewer_show_aabb() const;
    bool get_debug_viewer_show_obb() const;
    bool get_debug_viewer_show_anchor() const;

    std::shared_ptr<SceneGraph2D> get_scene_graph() const;
    std::shared_ptr<RenderSurface> get_render_surface() const;
    std::shared_ptr<FontManagerTTF> get_font_manager() const;

    int get_transform_recalculation_count();

    std::shared_ptr<Circle2D> create_circle(const Vec2d position, const double radius, const Color4 color, const double line_thickness = 1.0) const;
    std::shared_ptr<Circle2D> create_circle(const double x, const double y, const double radius, const Color4 color, const double line_thickness = 1.0) const;
    
    std::shared_ptr<Ellipse2D> create_ellipse(const Vec2d position, const Vec2d radius, const Color4 color, const double line_thickness = 1.0) const;
    std::shared_ptr<Ellipse2D> create_ellipse(const double x, const double y, const double radius_x, const double radius_y, const Color4 color, const double line_thickness = 1.0) const;

    std::shared_ptr<Polyline2D> create_polyline(const Vec2d position, const std::vector<Vec2d> &points, const Color4 color, const double line_thickness = 1.0) const;
    std::shared_ptr<Polyline2D> create_polyline(const double x, const double y, const std::vector<Vec2d> &points, const Color4 color, const double line_thickness = 1.0) const;

    std::shared_ptr<Polygon2D> create_polygon(const Vec2d position, const std::vector<Vec2d> &points, const Color4 fill_color) const;
    std::shared_ptr<Polygon2D> create_polygon(const double x, const double y, const std::vector<Vec2d> &points, const Color4 fill_color) const;

    // std::shared_ptr<Bitmap2D> create_bitmap(const Vec2d position, const Bitmap &bm) const;
    // std::shared_ptr<Bitmap2D> create_bitmap(const double x, const double y, const Bitmap &bm) const 
    // {
    //     return create_bitmap(Vec2d { x, y }, bm);
    // };
    //
    // std::shared_ptr<Bitmap2D> create_bitmap(const Vec2d position, const Vec2i resolution) const;
    // std::shared_ptr<Bitmap2D> create_bitmap(const double x, const double y, const Vec2i resolution) const 
    // {
    //     return create_bitmap(Vec2d { x, y }, resolution);
    // };

    std::shared_ptr<Text2D> create_text(const Vec2d position, const std::string &text, const std::shared_ptr<FontTTF> font, const double font_size, const Color4 color) const;
    std::shared_ptr<Text2D> create_text(const double x, const double y, const std::string &text, const std::shared_ptr<FontTTF> font, const double font_size, const Color4 color) const;

private:

    std::vector<std::pair<std::shared_ptr<Primitive2D>, Matrix3x3d>> get_draw_queue() const;

    std::shared_ptr<RenderSurface> surface;
    std::shared_ptr<SceneGraph2D> scene_graph;
    std::shared_ptr<FontManagerTTF> font_manager;
    std::shared_ptr<DebugViewer> debug_viewer;

    std::shared_ptr<FontTTF> default_font;

    std::vector<std::pair<std::shared_ptr<Primitive2D>, Matrix3x3d>> draw_queue;

    RenderSurface::BlendMode blend_mode = RenderSurface::BlendMode::ALPHA;

    mutable double last_frame_time_us = 0.0;

    Vec2d viewport_scaling;
};

}
