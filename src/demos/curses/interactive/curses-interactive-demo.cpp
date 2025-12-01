#include <gfx/core/render-2D.h>
#include <gfx/surfaces/curses/curses-render-surface.h>

#include "demos/curses/curses-utils.h"
#include "demos/common/interactive/interactive-demo.h"
#include "demos/common/core/demo-utils.h"

namespace demos
{

using namespace gfx;

void run()
{
    init();
    set_bold(true);

    auto surface { std::make_shared<CursesRenderSurface>(get_screen_size() * 2) };
    InteractiveDemo demo(std::make_shared<gfx::Render2D>(surface, Vec2d { 2, 1 }));

    bool running = true;

    while (running)
    {
        clear();
        demo.render_frame(0);
        set_color(default_color::WHITE);
        std::vector<std::string> info { demo.info_text() };
        for (int i = 0; i < info.size(); ++i)
        {
            add_str({ 0, i }, info[i]);
        }
        refresh();

        int input { get_input() };
        if (input == 'q')
        {
            running = false;
        }
        else
        {
            demo.handle_input(input);
        }
    }

    end();
}

}

int main()
{
    demos::run();
}

