set(DEMOS_COMMON_ANIMATIONS_FIREWORKS_SOURCES
        fireworks-demo.cpp
        firework.cpp
        particle.cpp
)

add_library(demos_common_animations_fireworks STATIC
        ${DEMOS_COMMON_ANIMATIONS_FIREWORKS_SOURCES}
)

target_link_libraries(demos_common_animations_fireworks PRIVATE
        gfx
)

target_include_directories(demos_common_animations_fireworks PUBLIC
        ${INCLUDE_DIR}
)
