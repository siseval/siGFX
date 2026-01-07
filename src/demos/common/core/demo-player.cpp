#include <iostream>

#include "demos/common/core/demo-player.h"
#include "demos/common/animations/test-3D/test-3D-demo.h"
#include "demos/common/animations/star/star-demo.h"
#include "demos/common/animations/snake/snake-demo.h"
#include "demos/common/animations/fireworks/fireworks-demo.h"
#include "demos/common/animations/space/space-demo.h"
#include "demos/common/animations/video/video-demo.h"
#include "demos/common/animations/fractal/fractal-demo.h"
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
        render2D->load_font_directory("/Users/sigurdsevaldrud/documents/code/c++/gfx/assets/fonts");
        render2D->set_debug_font(render2D->get_font("gohu-regular"));
    } 
    catch (std::runtime_error &e) 
    {
        std::cerr << "Warning: Could not load font directory: " << e.what() << std::endl;
    }

    demos.emplace_back(std::make_shared<Test3DDemo>(render2D, render3D));
    demos.emplace_back(std::make_shared<StarDemo>(render2D));
    demos.emplace_back(std::make_shared<TextDemo>(render2D));
    // demos.emplace_back(std::make_shared<VideoDemo>(renderer));
    demos.emplace_back(std::make_shared<SnakeDemo>(render2D));
    demos.emplace_back(std::make_shared<BoidsDemo>(render2D));
    demos.emplace_back(std::make_shared<FractalDemo>(render2D));
    demos.emplace_back(std::make_shared<ShaderDemo>(render2D));
    demos.emplace_back(std::make_shared<FireworksDemo>(render2D));
    demos.emplace_back(std::make_shared<SpaceDemo>(render2D));

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
    render2D->set_resolution(new_resolution);
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
            render2D->set_enable_debug_viewer(!render2D->get_enable_debug_viewer());
            break;
        case '5':
            render2D->set_debug_viewer_show_aabb(!render2D->get_debug_viewer_show_aabb());
            break;
        case '6':
            render2D->set_debug_viewer_show_obb(!render2D->get_debug_viewer_show_obb());
            break;
        case '7':
            render2D->set_debug_viewer_show_anchor(!render2D->get_debug_viewer_show_anchor());
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
    render2D->clear_items();
    render3D->clear_items();

    if (demos[current_demo]->get_clear_color() != render2D->get_clear_color())
    {
        render2D->set_clear_color(demos[current_demo]->get_clear_color());
    }

    demos[current_demo]->init();
}

}
