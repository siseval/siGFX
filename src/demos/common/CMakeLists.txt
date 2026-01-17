add_subdirectory(core)
add_subdirectory(interactive)
add_subdirectory(animations)

add_library(demos_common INTERFACE)

target_link_libraries(demos_common INTERFACE
        demos_common_core
        demos_common_interactive
        demos_common_animations
)

target_include_directories(demos_common INTERFACE
        ${INCLUDE_DIR}
)

