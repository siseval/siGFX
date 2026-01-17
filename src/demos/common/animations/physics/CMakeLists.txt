set(DEMOS_COMMON_ANIMATIONS_PHYSICS_SOURCES
        physics-demo.cpp
)

add_library(demos_common_animations_physics STATIC
        ${DEMOS_COMMON_ANIMATIONS_PHYSICS_SOURCES}
)

target_link_libraries(demos_common_animations_physics PRIVATE
        gfx
)

target_include_directories(demos_common_animations_physics PUBLIC
        ${INCLUDE_DIR}
)
