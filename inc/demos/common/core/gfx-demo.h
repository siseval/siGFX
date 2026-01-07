#pragma once

#include <string>
#include "gfx/core/render-engine.h"

namespace demos
{

enum class MouseEventType
{
    LEFT_DOWN,
    LEFT_UP,
    RIGHT_DOWN,
    RIGHT_UP,
    SCROLL_UP,
    SCROLL_DOWN,
    MOVE,
};

struct MouseEvent
{
    MouseEventType type;
    gfx::Vec2d position;
};

enum class KeyEventType
{
    KEY_PRESS,
    KEY_RELEASE,
    KEY_REPEAT,
};

enum class Key
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    W,
    A,
    S,
    D,
    Q,
    E,
    SHIFT,
    CTRL,
    SPACE,
    UNKNOWN
};

struct KeyEvent
{
    KeyEventType type;
    Key key;
};

class GfxDemo
{

public:

    GfxDemo(const std::shared_ptr<gfx::RenderEngine> renderer)
        : renderer(renderer)
    {
    }

    virtual void init() = 0;
    virtual void render_frame(const double dt) = 0;
    virtual void handle_char(const int input) = 0;
    virtual void report_mouse(const MouseEvent event) {}
    virtual void report_key(const KeyEvent event) {}
    virtual void end() = 0;

    virtual std::vector<std::string> debug_text() { return {}; }

    inline std::shared_ptr<gfx::RenderEngine> get_renderer() const { return renderer; }
    inline gfx::Vec2i get_resolution() const { return renderer->get_resolution(); }

    inline double get_fps() const 
    { 
        return 1000000 / last_frame_us; 
    }

    inline std::vector<std::string> info_text()
    {
        std::vector<std::string> info;
        info.push_back("resolution: " + std::to_string(renderer->get_resolution().round().x) + "x" + std::to_string(renderer->get_resolution().round().y));
        info.push_back("fps: " + std::to_string(static_cast<int>(get_fps())));
        info.push_back("items: " + std::to_string(renderer->num_primitives()));

        return info;
    }

    inline void set_last_frame_us(const double us) { last_frame_us = us; }
    inline virtual gfx::Color4 get_clear_color() const { return gfx::Color4(0.2, 0.2, 0.2); }

protected:


    std::shared_ptr<gfx::RenderEngine> renderer;
    double last_frame_us = 0.0;
};

}
