set(DEMOS_COMMON_ANIMATIONS_BOIDS_SOURCES
        boids-demo.cpp
)

add_library(demos_common_animations_boids STATIC
        ${DEMOS_COMMON_ANIMATIONS_BOIDS_SOURCES}
)

target_link_libraries(demos_common_animations_boids PRIVATE
        gfx
)

target_include_directories(demos_common_animations_boids PUBLIC
        ${INCLUDE_DIR}
)
