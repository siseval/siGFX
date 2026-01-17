set(DEMOS_COMMON_ANIMATIONS_SPACE_SOURCES
        space-demo.cpp
        simulate.cpp
        camera.cpp
        body.cpp
)

add_library(demos_common_animations_space STATIC
        ${DEMOS_COMMON_ANIMATIONS_SPACE_SOURCES}
)

target_link_libraries(demos_common_animations_space PRIVATE
        gfx
)

target_include_directories(demos_common_animations_space PUBLIC
        ${INCLUDE_DIR}
)
