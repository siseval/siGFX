#include "gfx/debug/debug-viewer.h"

#include <numeric>

namespace gfx
{

void DebugViewer::update(std::shared_ptr<RenderEngine> render_engine)
{
    clear(render_engine);
    if (!enabled)
    {
        return;
    }
    populate(render_engine);
}

void DebugViewer::populate(std::shared_ptr<RenderEngine> render_engine)
{
    RendererInfo info { gather_renderer_info(render_engine) };

    if (font)
    {
        update_text(render_engine, info);
    }
}

DebugViewer::RendererInfo DebugViewer::gather_renderer_info(const std::shared_ptr<RenderEngine> render_engine)
{
    double current_time_ms { std::chrono::duration<double, std::milli>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count() };

    fps_history.push_back(current_time_ms - last_frame_timestamp_ms);
    if (fps_history.size() > fps_history_max_size)
    {
        fps_history.pop_front();
    }
    double acc { std::accumulate(fps_history.begin(), fps_history.end(), 0.0) };
    double fps { acc == 0.0 ? 0.0 : fps_history.size() / (acc / 1000.0) };

    RendererInfo info {
        .fps = fps,
        .num_items = render_engine->num_primitives()
    };

    last_frame_timestamp_ms = current_time_ms;
    return info;
}

void DebugViewer::update_text(std::shared_ptr<RenderEngine> render_engine, const RendererInfo& info)
{
    std::string debug_text;

    if (show_fps)
    {
        debug_text += "fps: " + std::to_string(static_cast<int>(info.fps)) + "\n";
    }

    if (show_num_items)
    {
        debug_text += "items: " + std::to_string(info.num_items) + "\n";
    }

    if (!text)
    {
        text = render_engine->create_text(
            10.0,
            10.0,
            debug_text,
            font,
            font_size,
            text_color
        );
    }
    else
    {
        text->set_text(debug_text);
        text->set_font(font);
        text->set_font_size(font_size);
        text->set_color(text_color);
    }

    if (!render_engine->contains_primitive(text))
    {
        render_engine->add_primitive(text);
    }
}

int DebugViewer::num_debug_items()
{
    return show_fps || show_num_items;
}

void DebugViewer::clear(std::shared_ptr<RenderEngine> render_engine)
{
    if (text && render_engine->contains_primitive(text))
    {
        render_engine->remove_primitive(text);
    }
}

void DebugViewer::set_enabled(const bool enable)
{
    enabled = enable;
}

void DebugViewer::set_font(const std::shared_ptr<FontTTF> fnt)
{
    font = fnt;
}

void DebugViewer::set_font_size(const double size)
{
    font_size = size;
}

void DebugViewer::set_text_color(const Color4 color)
{
    text_color = color;
}

void DebugViewer::set_show_fps(const bool show)
{
    show_fps = show;
}

void DebugViewer::set_show_num_items(const bool show)
{
    show_num_items = show;
}

void DebugViewer::set_text_position(const Vec2d position)
{
    text_position = position;
}

bool DebugViewer::get_enabled() const
{
    return enabled;
}

std::shared_ptr<FontTTF> DebugViewer::get_font() const
{
    return font;
}

double DebugViewer::get_font_size() const
{
    return font_size;
}

Color4 DebugViewer::get_text_color() const
{
    return text_color;
}

bool DebugViewer::get_show_fps() const
{
    return show_fps;
}

bool DebugViewer::get_show_num_items() const
{
    return show_num_items;
}

Vec2d DebugViewer::get_text_position() const
{
    return text_position;
}


}
