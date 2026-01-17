set(DEMOS_COMMON_ANIMATIONS_FRACTAL_SOURCES
        fractal-demo.cpp
)

add_library(demos_common_animations_fractal STATIC
        ${DEMOS_COMMON_ANIMATIONS_FRACTAL_SOURCES}
)

target_link_libraries(demos_common_animations_fractal PRIVATE
        gfx
)

target_include_directories(demos_common_animations_fractal PUBLIC
        ${INCLUDE_DIR}
)
