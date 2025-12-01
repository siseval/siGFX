#include "demos/curses/curses-demo-player.h"
#include "demos/curses/curses-utils.h"

int main()
{
    demos::init();
    demos::set_bold(true);

    demos::CursesDemoPlayer player;
    player.init();
    player.run();

    demos::end();
}
