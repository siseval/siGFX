add_subdirectory(common)

if (USE_CURSES)
    add_subdirectory(curses)
endif ()

if (USE_GLFW)
    add_subdirectory(glfw)
endif ()

add_library(demos INTERFACE)

target_link_libraries(demos INTERFACE demos_common)

if (USE_CURSES)
    target_link_libraries(demos INTERFACE demos_curses)
endif ()

if (USE_GLFW)
    target_link_libraries(demos INTERFACE demos_glfw)
endif ()

target_include_directories(demos INTERFACE
        ${INCLUDE_DIR}
)

