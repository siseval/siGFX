#pragma once

#include "gfx/core/render-engine.h"
#include "gfx/math/box2.h"
#include "gfx/core/types/obb-2D.h"
#include "gfx/core/types/color4.h"
#include "gfx/primitives/polyline-2D.h"
#include "gfx/primitives/circle-2D.h"
#include "gfx/primitives/text-2D.h"

#include <memory>
#include <deque>


namespace gfx
{

class DebugViewer
{

    struct RendererInfo
    {
        double fps;
        int num_items;
    };

    struct ItemInfo2D
    {
        UUID id;
        Box2d aabb;
        OBB2D obb;
        Vec2d anchor_point;
    };

    struct DebugRender2D
    {
        std::shared_ptr<Polyline2D> aabb;
        std::shared_ptr<Polyline2D> obb;
        std::shared_ptr<Circle2D> anchor;
    };

    // struct ItemInfo3D
    // {
    //     UUID id;
    //     Box3d aabb;
    //     OBB3D obb;
    //     Vec3d anchor_point;
    // };
    //
    // struct DebugRender3D
    // {
    //     std::shared_ptr<Polyline3D> aabb;
    //     std::shared_ptr<Polyline2D> obb;
    //     std::shared_ptr<Circle2D> anchor;
    // };

public:

    void update(std::shared_ptr<RenderEngine> render_engine);

    void set_enabled(const bool enable);
    void set_font(const std::shared_ptr<FontTTF> fnt);
    void set_font_size(const double size);
    void set_text_color(const Color4 color);
    void set_show_fps(const bool show);
    void set_show_num_items(const bool show);
    void set_text_position(const Vec2d position);

    bool get_enabled() const;
    std::shared_ptr<FontTTF> get_font() const;
    double get_font_size() const;
    Color4 get_text_color() const;
    bool get_show_fps() const;
    bool get_show_num_items() const;
    Vec2d get_text_position() const;

private:

    void populate(std::shared_ptr<RenderEngine> render_engine);
    void clear(std::shared_ptr<RenderEngine> render_engine);

    RendererInfo gather_renderer_info(const std::shared_ptr<RenderEngine> render_engine);
    void update_text(std::shared_ptr<RenderEngine> render_engine, const RendererInfo& info);
    int num_debug_items();

    bool enabled = false;

    bool show_fps = true;
    bool show_num_items = true;

    std::shared_ptr<FontTTF> font;
    double font_size = 16.0;
    Color4 text_color { Color4::white() };
    Vec2d text_position { 10.0, 10.0 };

    std::shared_ptr<Text2D> text;

    std::deque<double> fps_history;
    int fps_history_max_size = 20;
    double last_frame_timestamp_ms = 0.0;

};

}
