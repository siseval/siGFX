find_package(Curses REQUIRED)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

set(GFX_SURFACES_CURSES_SOURCES
        curses-render-surface.cpp
)

add_library(gfx_surfaces_curses STATIC ${GFX_SURFACES_CURSES_SOURCES})

target_link_libraries(gfx_surfaces_curses PUBLIC
        gfx_core
        ${CURSES_LIBRARIES}
)

target_include_directories(gfx_surfaces_curses PUBLIC
        ${INCLUDE_DIR}
)

