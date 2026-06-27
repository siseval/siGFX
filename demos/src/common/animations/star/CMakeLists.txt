set(DEMOS_COMMON_ANIMATIONS_STAR_SOURCES
        star-demo.cpp
)

add_library(demos_common_animations_star STATIC
        ${DEMOS_COMMON_ANIMATIONS_STAR_SOURCES}
)

target_link_libraries(demos_common_animations_star PRIVATE
        gfx
)

target_include_directories(demos_common_animations_star PUBLIC
        ${INCLUDE_DIR}
)
