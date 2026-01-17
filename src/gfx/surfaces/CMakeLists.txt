if (USE_CURSES)
    add_subdirectory(curses)
endif ()

if (USE_GLFW)
    add_subdirectory(glfw)
endif ()

add_library(gfx_surfaces INTERFACE)

if (TARGET gfx_surfaces_curses)
    target_link_libraries(gfx_surfaces INTERFACE gfx_surfaces_curses)
endif ()

if (TARGET gfx_surfaces_glfw)
    target_link_libraries(gfx_surfaces INTERFACE gfx_surfaces_glfw)
endif ()

target_include_directories(gfx_surfaces INTERFACE
        ${INCLUDE_DIR}
)
