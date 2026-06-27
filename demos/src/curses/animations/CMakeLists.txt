set(CURSES_ANIMATIONS_DEMO_SOURCES
        main.cpp
)

add_executable(curses-animations-demo
        ${CURSES_ANIMATIONS_DEMO_SOURCES}
)

target_link_libraries(curses-animations-demo PUBLIC
        demos_common_core
        gfx
)

target_include_directories(curses-animations-demo PUBLIC
        ${INCLUDE_DIR}
)
