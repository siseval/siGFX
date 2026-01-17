set(CURSES_INTERACTIVE_DEMO_SOURCES
        curses-interactive-demo.cpp
)

add_executable(curses-interactive-demo
        ${CURSES_INTERACTIVE_DEMO_SOURCES}
)

target_link_libraries(curses-interactive-demo PUBLIC
        demos_common_interactive
        demos_common_core
        gfx
)

target_include_directories(curses-interactive-demo PUBLIC
        ${INCLUDE_DIR}
)
