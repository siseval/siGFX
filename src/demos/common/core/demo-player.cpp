#include <iostream>

#include "demos/common/core/demo-player.h"
#include "demos/common/animations/test-3D/test-3D-demo.h"
#include "demos/common/animations/star/star-demo.h"
#include "demos/common/animations/snake/snake-demo.h"
#include "demos/common/animations/fireworks/fireworks-demo.h"
#include "demos/common/animations/space/space-demo.h"
// #include "demos/common/animations/video/video-demo.h"
// #include "demos/common/animations/fractal/fractal-demo.h"
#include "demos/common/animations/boids/boids-demo.h"
#include "demos/common/animations/shader/shader-demo.h"
#include "demos/common/animations/text/text-demo.h"

namespace demos
{

using namespace gfx;

void DemoPlayer::init()
{
    try 
    {
        renderer->load_font_directory("/Users/sigurdsevaldrud/documents/code/c++/gfx/assets/fonts");
        renderer->debug_viewer_set_font(renderer->get_font("gohu-regular"));
    } 
    catch (std::runtime_error &e) 
    {
        std::cerr << "Warning: Could not load font directory: " << e.what() << std::endl;
    }

    demos.emplace_back(std::make_shared<Test3DDemo>(renderer));
    demos.emplace_back(std::make_shared<StarDemo>(renderer));
    demos.emplace_back(std::make_shared<TextDemo>(renderer));
    // demos.emplace_back(std::make_shared<VideoDemo>(renderer));
    demos.emplace_back(std::make_shared<SnakeDemo>(renderer));
    demos.emplace_back(std::make_shared<BoidsDemo>(renderer));
    // demos.emplace_back(std::make_shared<FractalDemo>(renderer));
    demos.emplace_back(std::make_shared<ShaderDemo>(renderer));
    demos.emplace_back(std::make_shared<FireworksDemo>(renderer));
    demos.emplace_back(std::make_shared<SpaceDemo>(renderer));

    cycle_demo(0);
}

void DemoPlayer::run()
{
    double last_frame_timestamp_us { time_ms() };
    while (running)
    {
        if (screen_size_changed())
        {
            resize(get_screen_size());
        }

        double now_us { time_us() };
        double dt_sec { (now_us - last_frame_timestamp_us) / 1000000.0 };

        demos[current_demo]->render_frame(dt_sec);
        demos[current_demo]->set_last_frame_us(now_us - last_frame_timestamp_us);
        last_frame_timestamp_us = now_us;

        if (show_info)
        {
            draw_info();
        }

        int input { get_input() };
        handle_input(input);
        demos[current_demo]->handle_char(input);
    }
}

void DemoPlayer::resize(const gfx::Vec2i new_resolution)
{
    renderer->set_resolution(new_resolution);
    demos[current_demo]->init();
}

void DemoPlayer::handle_input(const int input)
{
    switch (input)
    {
        case 'q':
            running = false;
            break;
        case 'U':
            show_info = !show_info;
            break;
        case '3':
            show_debug = !show_debug;
            break;
        case '4':
            renderer->debug_viewer_enable(!renderer->is_debug_viewer_enabled());
            break;
        case '5':
            renderer->debug_viewer_show_aabb(!renderer->is_debug_viewer_showing_aabb());
            break;
        case '6':
            renderer->debug_viewer_show_obb(!renderer->is_debug_viewer_showing_obb());
            break;
        case '7':
            renderer->debug_viewer_show_anchor(!renderer->is_debug_viewer_showing_anchor());
            break;
        case 'N':
            cycle_demo(1);
            break;
        case 'P':
            cycle_demo(-1);
            break;
    }

}

std::vector<std::string> DemoPlayer::get_info()
{
    std::vector<std::string> info { demos[current_demo]->info_text() };

    if (show_debug)
    {
        info.push_back("");
        for (const auto &line : demos[current_demo]->debug_text())
        {
            info.push_back(line);
        }
    }

    info.push_back("");
    info.push_back("[p/n] to cycle (" + std::to_string(current_demo + 1) + "/" + std::to_string(demos.size()) + ")");
    info.push_back("[q] to quit");

    return info;
}

void DemoPlayer::cycle_demo(const int direction)
{
    demos[current_demo]->end();
    current_demo = (current_demo + direction + demos.size()) % demos.size();
    renderer->clear_scene();

    if (demos[current_demo]->get_clear_color() != renderer->get_clear_color())
    {
        renderer->set_clear_color(demos[current_demo]->get_clear_color());
    }

    demos[current_demo]->init();
}

}
