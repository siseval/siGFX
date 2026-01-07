#include "demos/common/interactive/interactive-demo.h"

namespace demos
{

using namespace gfx;

void InteractiveDemo::init()
{
}

void InteractiveDemo::render_frame(const double dt)
{
    double t0 { static_cast<double>(clock()) };
    render2D->clear_frame();
    render2D->draw_frame();
    render2D->present_frame();
    last_frame_us = static_cast<double>(clock()) - t0;
}

void InteractiveDemo::select(int index)
{
    auto selected { get_selected() };
    if (items.size() <= 0)
    {
        selected = nullptr;
        return;
    }
    if (selected != nullptr)
    {
        selected->set_depth(1);
    }
    selected_index = index % items.size();
    selected = get_selected();
    selected->set_depth(0);
}

void InteractiveDemo::remove_selected()
{
    auto selected { get_selected() };
    if (selected == nullptr) 
    { 
        return; 
    }
    render2D->remove_item(selected);
    items.erase(items.begin() + selected_index);
    for (int i = 0; i < items.size(); ++i)
    {
        if (!render2D->contains_item(items[i]))
        {
            items.erase(items.begin() + i);
            i--;
        }
    }
    if (items.size() <= 0)
    {
        selected_index = -1;
        selected = nullptr;
        return;
    }
    selected_index %= items.size();
    select(selected_index);
}

void InteractiveDemo::add_circle(const bool child)
{
    Vec2d center { get_resolution() / 2.0 };
    Color4 color { std::rand() % 255, std::rand() % 255, std::rand() % 255 };

    auto circle { render2D->create_circle(center, 10, color, 3) };
    circle->set_anchor({ 0.5, 0.5 });

    items.push_back(circle);

    if (child && get_selected() != nullptr)
    {
        circle->set_position({ 0, 0 });
        render2D->add_item(circle, get_selected());
        return;
    }
    render2D->add_item(circle);
}

void InteractiveDemo::add_ellipse(const bool child)
{
    Vec2d center { get_resolution() / 2.0 };
    Color4 color { std::rand() % 255, std::rand() % 255, std::rand() % 255 };

    auto ellipse { render2D->create_ellipse(center, { 10, 10 }, color, 3) };
    ellipse->set_anchor({ 0.5, 0.5 });

    items.push_back(ellipse);

    if (child && get_selected() != nullptr)
    {
        ellipse->set_position({ 0, 0 });
        render2D->add_item(ellipse, get_selected());
        return;
    }
    render2D->add_item(ellipse);
}

void InteractiveDemo::add_polyline(const bool child)
{
    Vec2d center { get_resolution() / 2.0 };
    Color4 color { std::rand() % 255, std::rand() % 255, std::rand() % 255 };

    auto polyline { render2D->create_polyline(center, { { 0, 0 }, { 20, 0 }, { 10, 10 }, { 5, 30 } }, color, 3) };
    polyline->set_close(true);
    polyline->set_rounded_corners(true);
    polyline->set_anchor({ 0.5, 0.5 });

    items.push_back(polyline);

    if (child && get_selected() != nullptr)
    {
        polyline->set_position({ 0, 0 });
        render2D->add_item(polyline, get_selected());
        return;
    }
    render2D->add_item(polyline);
}

void InteractiveDemo::end()
{
    render2D->clear_items();
    items.clear();
    selected_index = -1;
}

void InteractiveDemo::handle_char(const int input)
{
    auto selected { get_selected() };

    switch (input)
    {
        case 'q':
            end();
            break;

        // case 'k':
        //     {
        //         if (selected == nullptr) { break; }
        //         auto ellipse = std::dynamic_pointer_cast<Ellipse2D>(selected);
        //         ellipse->set_radius(ellipse->get_radius() + Vec2d { 0, 1 });
        //         break;
        //     }
        // case 'j':
        //     {
        //         if (selected == nullptr) { break; }
        //         auto ellipse = std::dynamic_pointer_cast<Ellipse2D>(selected);
        //         ellipse->set_radius(ellipse->get_radius() - Vec2d { 0, 1 });
        //         break;
        //     }
        // case 'l':
        //     {
        //         if (selected == nullptr) { break; }
        //         auto ellipse = std::dynamic_pointer_cast<Ellipse2D>(selected);
        //         ellipse->set_radius(ellipse->get_radius() + Vec2d { 1, });
        //         break;
        //     }
        // case 'h':
        //     {
        //         if (selected == nullptr) { break; }
        //         auto ellipse = std::dynamic_pointer_cast<Ellipse2D>(selected);
        //         ellipse->set_radius(ellipse->get_radius() - Vec2d { 1, 0 });
        //         break;
        //     }

        case 'k':
            if (selected == nullptr) { break; }
            selected->set_scale(selected->get_scale() + Vec2d { 0, 0.05 });
            break;
        case 'j':
            if (selected == nullptr) { break; }
            selected->set_scale(selected->get_scale() - Vec2d { 0, 0.05 });
            break;

        case 'l':
            if (selected == nullptr) { break; }
            selected->set_scale(selected->get_scale() + Vec2d { 0.05, 0 });
            break;
        case 'h':
            if (selected == nullptr) { break; }
            selected->set_scale(selected->get_scale() - Vec2d { 0.05, 0 });
            break;

        case 'w':
            if (selected == nullptr) { break; }
            selected->set_position(selected->get_position() + Vec2d { 0, -1 });
            break;
        case 'a':
            if (selected == nullptr) { break; }
            selected->set_position(selected->get_position() + Vec2d { -1, 0 });
            break;
        case 's':
            if (selected == nullptr) { break; }
            selected->set_position(selected->get_position() + Vec2d { 0, 1 });
            break;
        case 'd':
            if (selected == nullptr) { break; }
            selected->set_position(selected->get_position() + Vec2d { 1, 0 });
            break;

        case 'L':
            if (selected == nullptr) { break; }
            selected->set_rotation(selected->get_rotation() + 0.05);
            break;
        case 'H':
            if (selected == nullptr) { break; }
            selected->set_rotation(selected->get_rotation() - 0.05);
            break;

        // case 'i':
        //     if (selected == nullptr) { break; }
        //     selected->set_line_thickness(selected->get_line_thickness() - 1);
        //     break;
        // case 'o':
        //     if (selected == nullptr) { break; }
        //     selected->set_line_thickness(selected->get_line_thickness() + 1);
        //     break;
        //
        // case 'f':
        //     if (selected == nullptr) { break; }
        //     selected->set_fill(!selected->get_fill());
        //     break;

        case 'b':
            if (selected == nullptr) { break; }
            break;
        case 'B':
            if (selected == nullptr) { break; }
            break;

        case 'n':
            select((selected_index + 1) % items.size());
            break;

        case 'r':
            if (selected == nullptr) { break; }
            remove_selected();
            break;

        case 'R':
            render2D->clear_items();
            for (int i = 0; i < items.size(); ++i)
            {
                if (!render2D->contains_item(items[i]))
                {
                    items.erase(items.begin() + i);
                    i--;
                }
            }
            break;
            
        case 'e':
            add_ellipse(false);
            select(items.size() - 1);
            break;

        case 't':
            add_polyline(false);
            select(items.size() - 1);
            break;

        case 'c':
            add_circle(false);
            select(items.size() - 1);
            break;

        case 'E':
            add_ellipse(true);
            select(items.size() - 1);
            break;

        case 'T':
            add_polyline(true);
            select(items.size() - 1);
            break;

        case 'C':
            add_circle(true);
            select(items.size() - 1);
            break;
       }
    }

}
