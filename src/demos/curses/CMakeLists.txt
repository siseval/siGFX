add_subdirectory(interactive)
add_subdirectory(animations)

add_library(demos_curses INTERFACE)

find_package(Curses REQUIRED)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

target_link_libraries(demos_curses INTERFACE
        ${CURSES_LIBRARIES}
)

target_include_directories(demos_curses INTERFACE
        ${INCLUDE_DIR}
)

