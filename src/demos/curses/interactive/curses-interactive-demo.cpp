#include <gfx/core/render-engine.h>
#include <gfx/surfaces/curses/curses-render-surface.h>

#include "demos/common/interactive/interactive-demo.h"
#include "demos/curses/curses-utils.h"

namespace demos
{

    using namespace gfx;

    void run()
    {
        init();
        set_bold(true);

        auto surface { std::make_shared<CursesRenderSurface>(get_screen_size() * 2) };
        InteractiveDemo demo(std::make_shared<RenderEngine>(surface));

        bool running = true;

        while (running)
        {
            clear();
            demo.render_frame(0);
            set_color(default_color::WHITE);
            std::vector info { demo.info_text() };
            for (int i = 0; i < info.size(); ++i)
            {
                add_str({ 0, i }, info[i]);
            }
            refresh();

            const int input { get_input() };
            if (input == 'q')
            {
                running = false;
            }
            else
            {
                demo.handle_char(input);
            }
        }

        end();
    }

}

int main()
{
    demos::run();
}
