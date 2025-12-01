#pragma once

#include <gfx/core/render-2D.h>
#include <gfx/surfaces/curses/curses-render-surface.h>

#include "demos/common/core/demo-player.h"
#include "demos/curses/curses-utils.h"

namespace demos
{

class CursesDemoPlayer : public demos::DemoPlayer
{

public:

    CursesDemoPlayer() : demos::DemoPlayer()
    {
        auto surface { std::make_shared<gfx::CursesRenderSurface>(demos::get_screen_size() * 2) };
        renderer = std::make_shared<gfx::Render2D>(surface, gfx::Vec2d { 2, 1 });
    }

    gfx::Vec2i get_screen_size() override
    {
        return demos::get_screen_size() * 2;
    }

    int get_input() override
    {
        int input { demos::get_input() };
        if (input == KEY_MOUSE)
        {
            MEVENT e;
            if (getmouse(&e) == OK)
            {
                demos::MouseEvent event { curses_to_mouse_event(e) };
                demos[current_demo]->report_mouse(event);
            }
            refresh();
            return 0;
        }
        return input;
    }

    void draw_info() override
    {
        demos::set_bold(true);
        demos::set_color(demos::default_color::WHITE);
        std::vector<std::string> info = get_info();
        for (int i = 0; i < info.size(); ++i)
        {
            add_str({ 0, i }, info[i]);
        }
    }

private:

    demos::MouseEvent curses_to_mouse_event(const MEVENT e)
    {
        demos::MouseEvent event;
        switch (e.bstate)
        {
            case BUTTON1_PRESSED:
                event.type = demos::MouseEventType::LEFT_DOWN;
                break;

            case BUTTON3_PRESSED:
                event.type = demos::MouseEventType::RIGHT_DOWN;
                break;

            case BUTTON4_PRESSED:
                event.type = demos::MouseEventType::SCROLL_UP;
                break;

            case BUTTON5_PRESSED:
                event.type = demos::MouseEventType::SCROLL_DOWN;
                break;

            case REPORT_MOUSE_POSITION:
                event.type = demos::MouseEventType::MOVE;
                break;

            default:
                break;
        }
        gfx::Vec2d position = gfx::Vec2i { e.x, e.y } * 2 / renderer->get_viewport_scaling();
        event.position = position / renderer->get_resolution();
        return event;
    }

};

}
